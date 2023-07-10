/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 20:10:55 by hbalasan          #+#    #+#             */
/*   Updated: 2023/07/08 20:52:48 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "get_next_line.h"

typedef struct s_pipex
{
    int     argc;
    char    **argv;
    char    **env;
    int     **pipes;
    int     filein;
    int     fileout;
} t_pipex;

//____________________/ft_split.c/____________________//
char	**ft_split(char const *s, char c);
//_____________________/pipex.c/_____________________//
int     open_file(char *argv, int i);
// void    heredoc(t_pipex pipex, char *delimiter);
void    close_and_free(int **pipes, int i);
//____________________/pipex_utils___________________//
void    error(void);
int     slash_check(char *cmd);
char    *find_path(char *cmd, char **env);
void    execute(char *argv, char **env);
//____________________/utils/________________________//
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void    close_the_pipes(t_pipex pipex, int i);

void	to_free(int **arr, int j);

void    process_new_heredoc(t_pipex pipex, int fd, int i);
void    process_new(t_pipex pipex, int i);

#endif