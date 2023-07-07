/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 20:10:50 by hbalasan          #+#    #+#             */
/*   Updated: 2023/07/07 21:05:14 by hbalasan         ###   ########.fr       */
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
// void    process_new_heredoc(t_pipex *pipex, int fd, char *argv, int i)
// {
//     pid_t   id;

//     id = fork();
//     if (id == -1)
//         error();
//     printf ("asjhbfhfbfwf\n");
//     if (id == 0)
//     {
//         if (i == 3)
//         {
//             if (fd == -1)
//                error();
//                 // exit(EXIT_FAILURE);
//             dup2(fd, STDIN_FILENO);
//             dup2(pipex->pipes[i - 2][1], STDOUT_FILENO);
//         }
//         else if (i == pipex->argc - 2)
//         {
//             dup2(pipex->pipes[i - 3][0], STDIN_FILENO);
//             dup2(pipex->fileout, STDOUT_FILENO);
//         }
//         else
//         {
//             dup2(pipex->pipes[i - 3][0], STDIN_FILENO);
//             dup2(pipex->pipes[i - 2][1], STDOUT_FILENO);
//         }
//         dprintf (2, "sgsdgdsgsg\n");
//         close(pipex->filein);
//         close(pipex->fileout);
//         int i = -1;
//         while (++i < pipex->argc - 4)
//         {
//             close(pipex->pipes[i][0]);
//             close(pipex->pipes[i][1]);
//             free(pipex->pipes[i]);
//         }
//         free(pipex->pipes);
//         execute(argv, pipex->env);
//     }
// }

// void    heredoc(t_pipex *pipex, char *delimiter)
// {
//     int     fd[2];
//     char    *line;
//     int     i;

//     i = 3;
//     if (pipe(fd) < 0)
//         error();
//     while (1)
//     {
//         write (1, "> ", 2);
//         line = get_next_line(0);
//         if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
//             break ;
//         write(fd[1], line, ft_strlen(line));
//     }
//     process_new_heredoc(pipex, fd[0], pipex->argv[i], i);
// }

void    process_new_heredoc(t_pipex pipex, int fd, char *argv, int i)
{
    pid_t   id;

    id = fork();
    if (id == -1)
        error();
    printf ("asjhbfhfbfwf\n");
    dprintf (2, "fd: %d\n", fd);
    if (id == 0)
    {
        if (i == 3)
        {
            if (fd == -1)
               error();
                // exit(EXIT_FAILURE);
            dup2(fd, STDIN_FILENO);
            dup2(pipex.pipes[i - 3][1], STDOUT_FILENO);
        }
        else if (i == pipex.argc - 2)
        {
            dup2(pipex.pipes[i - 4][0], STDIN_FILENO);
            dup2(pipex.fileout, STDOUT_FILENO);
        }
        else
        {
            dup2(pipex.pipes[i - 4][0], STDIN_FILENO);
            dup2(pipex.pipes[i - 3][1], STDOUT_FILENO);
        }
        dprintf (2, "sgsdgdsgsg\n");
        close(pipex.filein);
        close(pipex.fileout);
        int j = -1;
        while (++j < pipex.argc - 4)
        {
            close(pipex.pipes[j][0]);
            close(pipex.pipes[j][1]);
            free(pipex.pipes[j]);
        }
        free(pipex.pipes);
        execute(argv, pipex.env);
    }
}

void    heredoc(t_pipex pipex, char *delimiter)
{
    int     fd[2];
    char    *line;
    int     i;

    i = 3;
    if (pipe(fd) < 0)
        error();
    while (1)
    {
        write (1, "> ", 2);
        line = get_next_line(0);
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
            break ;
        printf ("line: %s\n", line);
        write(fd[1], line, ft_strlen(line));
    }
    free(line);
    // free(delimiter);
    while (i < pipex.argc - 4)
    {
        process_new_heredoc(pipex, fd[0], pipex.argv[i], i);
        i++;
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
        file = open(argv, O_RDONLY);
    // if (file == -1)
    // {
    //     perror("PIPEX error");
    //     return (-1);
    // }
    return (file); 
}

void    process_new(t_pipex pipex, char *argv, int i)
{
    pid_t   id;

    id = fork();
    if (id == -1)
        error();
    if (id == 0)
    {
        if (i == 2)
        {
            if (pipex.filein == -1)
                exit(EXIT_FAILURE);
            dup2(pipex.filein, STDIN_FILENO);
            dup2(pipex.pipes[i - 2][1], STDOUT_FILENO);
        }
        else if (i == pipex.argc - 2)
        {
            dup2(pipex.pipes[i - 3][0], STDIN_FILENO);
            dup2(pipex.fileout, STDOUT_FILENO);
        }
        else
        {
            dup2(pipex.pipes[i - 3][0], STDIN_FILENO);
            dup2(pipex.pipes[i - 2][1], STDOUT_FILENO);
        }
        close(pipex.filein);
        close(pipex.fileout);
        int i = -1;
        while (++i < pipex.argc - 4)
        {
            close(pipex.pipes[i][0]);
            close(pipex.pipes[i][1]);
            free(pipex.pipes[i]);
        }
        free(pipex.pipes);
        execute(argv, pipex.env);
    }
}

int main(int argc, char **argv, char **env)
{
    int     i;
    t_pipex pipex;
    
    pipex.argc = argc;
    pipex.argv = argv;
    pipex.env = env;
    if (argc >= 5)
    {
        if (ft_strncmp(argv[1], "here_doc", 8) == 0)
        {
            i = 2;
            // printf ("yhausvfgasfvsuyavdasuyvsauvdsaudvsauydg\n");
            pipex.fileout = open_file(argv[argc - 1], 0);
            heredoc(pipex, argv[2]);
        }
        else
        {
            pipex.pipes = (int **)malloc(sizeof(int *) * (argc - 4));
            i = 0;
            while (i < argc - 4)
            {
                pipex.pipes[i] = (int *)malloc(sizeof(int) * 2);
                if (pipex.pipes[i])  
                {
                    if (pipe(pipex.pipes[i]) == -1) //fd[0] -> the read end, fd[1] -> write end;
                    {
                        free(pipex.pipes[i]);
                        close_and_free(pipex.pipes, i);
                        free(pipex.pipes);
                        error();
                    }
                }
                else if (!pipex.pipes[i] && i == 0)
                    free(pipex.pipes);
                i++;
            }
            i = 1;
            pipex.filein = open_file(argv[1], 2);
            pipex.fileout = open_file(argv[argc - 1], 1);
            while (++i < argc - 1)
            {
                process_new(pipex, argv[i], i);
            }
        }
        i = -1;
        while (++i < argc - 4)
        {
            close(pipex.pipes[i][0]);
            close(pipex.pipes[i][1]);
            free(pipex.pipes[i]);
        }
        free(pipex.pipes);
        while (wait(NULL) != -1)
            ;
    }
    else
       error();
}