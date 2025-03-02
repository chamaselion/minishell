/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:11:50 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:11:50 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*strndup(const char *s, size_t n)
{
	char	*dup;

	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (0);
	ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}

static void	normbs2(char **result, int i)
{
	while (i--)
		free(result[i]);
	free(result);
}

char	**ft_split(char const *s, char c)
{
	t_SplitData	data;

	if (!s)
		return (0);
	data.word_count = count_words(s, c);
	data.result = (char **)malloc(sizeof(char *) * (data.word_count + 1));
	if (!data.result)
		return (0);
	data.i = 0;
	while (*s && data.i < data.word_count)
	{
		while (*s && *s == c)
			s++;
		data.start = s;
		while (*s && *s != c)
			s++;
		data.result[data.i++] = strndup(data.start, s - data.start);
		if (!data.result[data.i - 1])
		{
			normbs2(data.result, data.i);
			return (0);
		}
	}
	data.result[data.i] = 0;
	return (data.result);
}
