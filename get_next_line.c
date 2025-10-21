/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tloin <tloin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 12:58:09 by tloin             #+#    #+#             */
/*   Updated: 2025/10/21 15:29:35 by tloin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#ifndef BUFFER_SIZE
# define GNL_BUFFER_SIZE 32
#elif BUFFER_SIZE > 0
# define GNL_BUFFER_SIZE BUFFER_SIZE
#else
# define GNL_BUFFER_SIZE 32
#endif

static char	*read_into_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	r;

	buf = malloc((size_t)GNL_BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	r = 1;
	while (r > 0 && !ft_strchr(stash, '\n'))
	{
		r = read(fd, buf, (size_t)GNL_BUFFER_SIZE);
		if (r == -1)
			return (free(buf), free(stash), NULL);
		buf[r] = '\0';
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (free(buf), NULL);
	}
	return (free(buf), stash);
}

static char	*extract_line(char *stash)
{
	size_t	i;
	char	*line;

	if (!stash || !*stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = ft_substr(stash, 0, i + 1);
	else
		line = ft_substr(stash, 0, i);
	return (line);
}

static char	*update_stash(char *stash)
{
	size_t	i;
	size_t	len;
	char	*new;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	len = ft_strlen(stash + i + 1);
	new = ft_substr(stash, i + 1, len);
	return (free(stash), new);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*line;

	if (fd < 0 || fd >= 1024 || (size_t)GNL_BUFFER_SIZE == 0)
		return (NULL);
	stash[fd] = read_into_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = extract_line(stash[fd]);
	stash[fd] = update_stash(stash[fd]);
	return (line);
}
