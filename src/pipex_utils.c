/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:53:21 by hbalasan          #+#    #+#             */
/*   Updated: 2023/05/22 18:19:18 by hbalasan         ###   ########.fr       */
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
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
            break ;
        i++;
    }
    if (!env[i])
        return (NULL);
    paths = ft_split(env[i] + 5, ':');
    i = 0;
    while (paths[i])
    {
        path_part = ft_strjoin(paths[i], "/");
        path = ft_strjoin(path_part, cmd);
        //free(path_part);

        if (access(path, F_OK) == 0)
            return (path);
        //free(path);
        i++;
    }
    free(paths);
    return (NULL);
}

void    execute(char *argv, char **env)
{
    char    **cmd;
    char    *path;
    int     i;

    cmd = ft_split(argv, ' ');
    if (slash_check(cmd[0]) == 0)
    {

        path = find_path(cmd[0], env);
        i = 0;
        if (!path)
        {
            while (cmd[i])
                free(cmd[i++]);
            free(cmd);
        }
    }
    execve(path, cmd, env);
    write (2, "Error: Command not found\n", 25);
    exit (1);
    // if (execve(path, cmd, env) == -1)
    //     error();
}