/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 20:10:50 by hbalasan          #+#    #+#             */
/*   Updated: 2023/06/22 20:03:32 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    close_and_free(int **pipes, int i)
{
    int j;

    j = 0;
    while (j < i)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
        free(pipes[j]);
        j++;
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
        close(fd[1]);
        dup2(fd[0], STDOUT_FILENO); //STDOUT_FILENO - 1
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

void    process_new(int **pipes, char *argv, char **env, int i, int filein, int fileout, int argc) //FIX THISSSSSSSSSSS !! ! !!1!!!1
{
    pid_t   id;
    // int     j;

    // j = i - 2;
    printf ("command num : %d, %s\n", i, argv);
    id = fork();
    if (id == -1)
        error();
    if (id == 0)
    {
        if (i == 2)
        {
            dup2(filein, STDIN_FILENO);
            dup2(pipes[i - 2][1], STDOUT_FILENO);
        }
        else if (i == argc - 2)
        {
            dup2(pipes[i - 3][0], STDIN_FILENO);
            dup2(fileout, STDOUT_FILENO);
        }
        else
        {
            dup2(pipes[i - 3][0], STDIN_FILENO);
            dup2(pipes[i - 2][1], STDOUT_FILENO);
        }
        close(filein);
        close(fileout);
        int i = -1;
        while (++i < argc - 4)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
        }
        free(pipes);
        execute(argv, env);
    }
}

int main(int argc, char **argv, char **env)
{
    int i;
    int filein;
    int fileout;
    int **pipes;
    
    if (argc >= 5)
    {
        if (ft_strncmp(argv[1], "here_doc", 8) == 0)
        {
            i = 2;
            fileout = open_file(argv[argc - 1], 0);
            heredoc(argv[2], argc);
        }
        else
        {
            pipes = (int **)malloc(sizeof(int *) * (argc - 4));
            i = 0;
            while (i < argc - 4)
            {
                pipes[i] = (int *)malloc(sizeof(int) * 2);
                if (pipes[i])  
                {
                    if (pipe(pipes[i]) == -1) //fd[0] -> the read end, fd[1] -> write end;
                    {
                        free(pipes[i]);
                        close_and_free(pipes, i);
                        free(pipes);
                        error();
                    }
                }
                else if (!pipes[i] && i == 0)
                    free(pipes);
                else
                {
                    close_and_free(pipes, i);
                    free(pipes);
                    error();
                }
                i++;
            }
            i = 1;
            filein = open_file(argv[1], 2);
            fileout = open_file(argv[argc - 1], 1);
            // process_new(pipes, NULL, NULL, i, filein, argc);
            // i++;
            //........................
        }
        
        while (++i < argc - 1)
        {
            process_new(pipes, argv[i], env, i, filein, fileout, argc);
        }
        i = -1;
        while (++i < argc - 4)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
        }
        free(pipes);
        while (wait(NULL) != -1)
            ;
    }
    else
       error();
}