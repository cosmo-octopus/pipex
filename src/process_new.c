/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 19:37:22 by hbalasan          #+#    #+#             */
/*   Updated: 2023/07/10 20:01:51 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	redirect_io(t_pipex pipex, int i)
{
	if (i == 2)
	{
		if (dup2(pipex.filein, STDIN_FILENO) < 0 \
		|| dup2(pipex.pipes[i - 2][1], STDOUT_FILENO) < 0)
			error();
	}
	else if (i == pipex.argc - 2)
	{
		if (dup2(pipex.pipes[i - 3][0], STDIN_FILENO) < 0 \
		|| dup2(pipex.fileout, STDOUT_FILENO) < 0)
			;
	}
	else
		if (dup2(pipex.pipes[i - 3][0], STDIN_FILENO) < 0 \
		|| dup2(pipex.pipes[i - 2][1], STDOUT_FILENO) < 0)
			error();
}

void	redirect_io_heredoc(t_pipex pipex, int i, int fd)
{
	if (i == 3)
	{
		if (dup2(fd, STDIN_FILENO) < 0 \
		|| dup2(pipex.pipes[i - 3][1], STDOUT_FILENO) < 0)
			error();
	}
	else if (i == pipex.argc - 2)
	{
		if (dup2(pipex.pipes[i - 4][0], STDIN_FILENO) < 0 \
		|| dup2(pipex.fileout, STDOUT_FILENO) < 0)
			error();
	}
	else
		if (dup2(pipex.pipes[i - 4][0], STDIN_FILENO) < 0 \
		|| dup2(pipex.pipes[i - 3][1], STDOUT_FILENO) < 0)
			error();
}

void	close_and_free(t_pipex pipex)
{
	int	i;

	i = -1;
	while (++i < pipex.argc - 4)
	{
		close(pipex.pipes[i][0]);
		close(pipex.pipes[i][1]);
		free(pipex.pipes[i]);
	}
	free(pipex.pipes);
}

void	process_new(t_pipex pipex, int i)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		error();
	if (id == 0)
	{
		redirect_io(pipex, i);
		close(pipex.filein);
		close(pipex.fileout);
		close_and_free(pipex);
		execute(pipex.argv[i], pipex.env);
	}
}

void	process_new_heredoc(t_pipex pipex, int fd, int i)
{
	pid_t	id;

	id = fork();
	if (id < 0)
		error();
	if (id == 0)
	{
		redirect_io_heredoc(pipex, i, fd);
		close(fd);
		close(pipex.fileout);
		close_and_free(pipex);
		execute(pipex.argv[i], pipex.env);
	}
}
