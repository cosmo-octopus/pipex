/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbalasan <hbalasan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:06:33 by hbalasan          #+#    #+#             */
/*   Updated: 2023/07/10 19:38:25 by hbalasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	**ft_free(char **str, size_t j)
{
	while (j)
		free(str[j--]);
	free(str);
	return (NULL);
}

static char	**fill_string(char **split, char const *s, char c)
{
	size_t	len;
	size_t	i;

	i = -1;
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c)
			{
				len++;
				s++;
			}
			split[++i] = ft_substr(s - len, 0, len);
			if (split[i] == NULL)
				return (ft_free(split, i - 1));
		}
		else
			s++;
	}
	split[++i] = NULL;
	return (split);
}

static size_t	count_words(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		i++;
		while (*s && *s == c)
			s++;
		if (*s == '\0')
			i--;
		while (*s && *s != c)
			s++;
	}
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	split = fill_string(split, s, c);
	return (split);
}
