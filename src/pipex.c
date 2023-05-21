/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 20:00:13 by hbalasan          #+#    #+#             */
/*   Updated: 2023/05/21 18:21:51 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    error(void)
{
    perror("Error");
    exit(EXIT_FAILURE);
}

void    process_new(char *argv, char **env)
{
    pid_t   id;
    int     fd[2];


    if (pipe(fd) == -1)
    {
        error();
    }
    id = fork();
    if (id == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execute(argv, env);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], 0);
        wait(NULL);
    }
}

void    heredoc(char *delimiter, int argc)
{
    pid_t   id;
    int     fd[2];
    char    *line;

    argc = 1;
    if (pipe(fd) == -1)
        error();
    id = fork();
    if (id == 0)
    {
        close(fd[0]);
        while (1)
        {
            line = get_next_line(0);
            if (!line)
                return ;
            if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
                exit(EXIT_SUCCESS);
            write(fd[1], line, ft_strlen(line));
        }
    }
    else
    {
        //close(fd[1]);
        //dup2(fd[0], 1);
        wait(NULL);
    }
}

int open_file(char *argv, int i)
{
    int file;

    file = 0;
    if (i == 0)
        file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0755);
    else if (i == 1)
       file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    else if (i == 2)
        file = open(argv, O_RDONLY, 0755);
    if (file == -1)
        error();
    return (file); 
}

/*
./pipex infile cat ls outfile
open()
PATH=/...:/...:
access( ---> check in PATH if no /
execve


*/

int main(int argc, char **argv, char **env)
{
    int i;
    int filein;
    int fileout;
    
    if (argc >= 5)
    {
        if (ft_strncmp(argv[1], "here_doc", 8) == 0)
        {
            i = 3;
            fileout = open_file(argv[argc - 1], 0);
            heredoc(argv[2], argc);
        }
        else
        {
            i = 2;
            filein = open_file(argv[1], 2);
            fileout = open_file(argv[argc - 1], 1);
            dup2(filein, STDIN_FILENO); //STDIN_FILENO - 0
        }
        while (i < argc - 2)
        {
            printf("%s\n", argv[i]);
            process_new(argv[i++], env);
        }
        dup2(fileout, STDOUT_FILENO);
        execute(argv[argc - 2], env);
    }
    error();
    //return;
}