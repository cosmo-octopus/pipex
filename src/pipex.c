#include "../includes/pipex.h"

void    heredoc(t_pipex pipex, char *delimiter)
{
    int     fd[2];
    char    *line;
    int     i;

    if (pipe(fd) == -1)
        error();
    while (1)
    {
        line = get_next_line(STDIN_FILENO);
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)))
            break;
        write(fd[1], line, ft_strlen(line));
    }
    i = 2;
    while (++i < pipex.argc - 1)
    {
        process_new_heredoc(pipex, fd[0], i);
    }
    i = -1;
    while (++i < pipex.argc - 4)
    {
        close(pipex.pipes[i][0]);
        close(pipex.pipes[i][1]);
        free(pipex.pipes[i]);
    }
    free(pipex.pipes);
    while (wait(NULL) != -1)
        ;
    close(fd[0]);
    close(fd[1]);
    close(pipex.fileout);
}

static int fds_and_pipes(t_pipex pipex)
{
    int i;

    i = -1;
    while (++i < pipex.argc - 4)
    {
        if (pipe(pipex.pipes[i]) < 0)
        {
            close_the_pipes(pipex, i);
            to_free(pipex.pipes, pipex.argc - 4);
            return (-1);
        }
    }
    return (0);
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
    return (file); 
}

int main (int argc, char **argv, char **env)
{
    int     i;
    t_pipex pipex;

    pipex.argc = argc;
    pipex.argv = argv;
    pipex.env = env;
    if (argc >= 5)
    {
        pipex.pipes = (int **)malloc(sizeof(int *) * (argc - 4));
        i = -1;
        while (++i < argc - 4)
        {
            pipex.pipes[i] = (int *)malloc(sizeof(int *) * 2);
            if (!pipex.pipes[i])
                to_free(pipex.pipes, i);
        }
        if (fds_and_pipes(pipex) == -1)
            error();
        if (strncmp(argv[1], "here_doc", 8) == 0)
        {
            pipex.fileout = open_file(argv[argc - 1], 0);
            heredoc(pipex, argv[2]);
        }
        else
        {
            pipex.filein = open_file(argv[1], 2);
            pipex.fileout = open_file(argv[argc - 1], 1);
            i = 2;
            while (i < argc - 1)
            {
                process_new(pipex, i++);
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
            close(pipex.filein);
            close(pipex.fileout);
        }
    }
    else
        error();
}