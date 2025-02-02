/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:09:39 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:09:39 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*s1cc;

	i = 0;
	s1cc = (char *)malloc((n + 1) * sizeof(char));
	if (s1cc == NULL)
	{
		return (NULL);
	}
	while (i < n && s1[i] != '\0')
	{
		s1cc[i] = s1[i];
		i++;
	}
	s1cc[i] = '\0';
	return (s1cc);
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	is_pipe(char *str)
{
	if (!str)
		return (0);
	return (*str == '|');
}

int	identify_env_var(char *str)
{
	return (*str == '$' && (*(str + 1) == '?' || (*(str + 1)
				&& !is_whitespace(*(str + 1)))));
}

int	is_redirection(char *str)
{
	if (!str)
		return (0);
	return (*str == '<' || *str == '>');
}
