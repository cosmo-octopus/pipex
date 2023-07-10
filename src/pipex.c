/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 18:53:41 by hbalasan          #+#    #+#             */
/*   Updated: 2023/07/10 20:28:03 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	heredoc(t_pipex pipex, char *delimiter)
{
	int		fd[2];
	char	*line;
	int		i;

	if (pipe(fd) == -1)
		error();
	pipex.fileout = open_file(pipex.argv[pipex.argc - 1], 0);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
	close(fd[1]);
	i = 2;
	while (++i < pipex.argc - 1)
		process_new_heredoc(pipex, fd[0], i);
	close(fd[0]);
}

static int	fds_and_pipes(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->argc - 4)
	{
		if (pipe(pipex->pipes[i]) < 0)
		{
			close_the_pipes(*pipex, i);
			to_free(pipex->pipes, pipex->argc - 4);
			return (-1);
		}
	}
	return (0);
}

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0755);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	else if (i == 2)
		file = open(argv, O_RDONLY);
	return (file);
}

void	make_pipex_arr(t_pipex *pipex)
{
	int	i;

	pipex->pipes = (int **)malloc(sizeof(int *) * (pipex->argc - 4));
	i = -1;
	while (++i < pipex->argc - 4)
	{
		pipex->pipes[i] = (int *)malloc(sizeof(int *) * 2);
		if (!pipex->pipes[i])
			to_free(pipex->pipes, i);
	}
	if (fds_and_pipes(pipex) == -1)
		error();
}

int	main(int argc, char **argv, char **env)
{
	int		i;
	t_pipex	pipex;

	pipex.argc = argc;
	pipex.argv = argv;
	pipex.env = env;
	if (argc < 5)
		error();
	make_pipex_arr(&pipex);
	if (argc > 5 && ft_strcmp(argv[1], "here_doc") == 0)
		heredoc(pipex, argv[2]);
	else
	{
		pipex.filein = open_file(argv[1], 2);
		pipex.fileout = open_file(argv[argc - 1], 1);
		i = 1;
		while (++i < argc - 1)
			process_new(pipex, i);
		close(pipex.filein);
	}
	close_and_free(pipex);
	close(pipex.fileout);
	while (wait(NULL) != -1)
		;
}
