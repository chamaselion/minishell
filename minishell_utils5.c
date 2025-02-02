/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <mnaumann@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:54:57 by mnaumann          #+#    #+#             */
/*   Updated: 2025/02/02 09:32:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_and_free2(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s2);
	return (result);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((unsigned char)s1[i]
		&& (unsigned char)s1[i] == (unsigned char)s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_builtin_command(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "unset", "export", "env",
		"exit"};
	int			i;
	if (!cmd)
		return (0);
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
