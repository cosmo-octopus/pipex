/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:53:21 by hbalasan          #+#    #+#             */
/*   Updated: 2023/05/12 20:32:08 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pipex.h"

int slash_check(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '/')
            return (1);
        i++;
    }
    return (0);
}

char *find_path(char *cmd, char **env)
{
    char    **paths;
    char    *path;
    char    *path_part;
    int     i;

    i = 0;
    while (ft_strncmp(env[i], "PATH", 4) == 0)
        i++;
    //if (!env[i])
    paths = ft_split(env[i + 5], ':');
    i = 0;
    while (paths[i])
    {
        path_part = ft_strjoin(paths[i], "/");
        path = ft_strjoin(path_part, cmd);
        free(path_part);
        if (access(path, F_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    i = -1;
    while (paths[++i])
        free(paths[i]);
    free(paths);
    return (NULL);
}

void    execute(char *path, char **env)
{
    
}