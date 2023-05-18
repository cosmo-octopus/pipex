/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:48:57 by hbalasan          #+#    #+#             */
/*   Updated: 2023/05/18 20:50:01 by hbalasan         ###   ########.fr       */
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

//_______________________________/ft_split/_______________________________//
char	**ft_split(char const *s, char c);
//_______________________________/utils/__________________________________//
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
//_______________________________/pipex_utils/____________________________//
int     slash_check(char *cmd);
char    *find_path(char *cmd, char **env);
void    execute(char *argv, char **env);
//_______________________________/pipex/_________________________________//
int     open_file(char *argv, int i);
void    heredoc(char *delimiter, int argc);
void    process_new(char *argv, char **env);
void    error(void);

#endif