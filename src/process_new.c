#include "../includes/pipex.h"

void    process_new(t_pipex pipex, int i)
{
    pid_t   id;
    int     j;

    id = fork();
    if (id < 0)
        error();
    if (id == 0)
    {
        if (i == 2)
        {
            if (pipex.filein < 0)
                error();
            dup2(pipex.filein, STDIN_FILENO);
            dup2(pipex.pipes[i - 2][1], STDOUT_FILENO);
        }
        else if (i == pipex.argc - 2)
        {
            dprintf (2, "out: %d\n", dup2(pipex.pipes[i - 3][0], STDIN_FILENO));
            // dup2(pipex.pipes[i - 3][0], STDIN_FILENO);
            dprintf (2, "in: %d\n", dup2(pipex.fileout, STDOUT_FILENO));
        }
        else
        {
            dup2(pipex.pipes[i - 3][0], STDIN_FILENO);
            dup2(pipex.pipes[i - 2][1], STDOUT_FILENO);
        }
        close(pipex.filein);
        close(pipex.fileout);
        j = -1;
        while (++j < pipex.argc - 4)
        {
            close(pipex.pipes[j][0]);
            close(pipex.pipes[j][1]);
            free(pipex.pipes[j]);
        }
        free(pipex.pipes);
        execute(pipex.argv[i], pipex.env);
    }
}

void    process_new_heredoc(t_pipex pipex, int fd, int i)
{
    pid_t   id;
    int     j;

    id = fork();
    if (id < 0)
        error();
    if (id == 0)
    {
        if (i == 3)
        {
            dup2(fd, STDIN_FILENO);
            dup2(pipex.pipes[i - 3][1], STDOUT_FILENO);
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
        close(fd);
        close(pipex.fileout);
        j = -1;
        while (++j < pipex.argc - 4)
        {
            close(pipex.pipes[j][0]);
            close(pipex.pipes[j][1]);
            free(pipex.pipes[j]);
        }
        free(pipex.pipes);
        execute(pipex.argv[i], pipex.env);
    }
}
