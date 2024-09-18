/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:12:45 by root              #+#    #+#             */
/*   Updated: 2024/09/18 11:28:53 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char *str, e_special_char type)
{
	if (str == NULL || type == NULL)
		return (0);
	return (*str == (char)type);
// implement the rest of the function, recognizing the special characters
// return (1) if character is found, (0) if not, used lika a bool later on in combo with the status
// CAUTION with multichar special characters, e.g. '>>' or '&&' and escape char "\"
}

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*end;

	if (str == NULL)
		str = *saveptr;
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == '\0')
	{
		*saveptr = str;
		return (NULL);
	}
	end = str;
	while (*end && !ft_strchr(delim, *end))
		end++;
	if (*end == '\0')
	{
		*saveptr = end;
		return (str);
	}
	*end = '\0';
	*saveptr = end + 1;
	return (str);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;

	return (ft_strtok_r(str, delim, &last));
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}