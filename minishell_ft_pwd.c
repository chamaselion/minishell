/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:04:11 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 15:32:13 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = malloc(sizeof(char) * (32768 + 1));
	if (!pwd)
		return (1);
	if (getcwd(pwd, 32768))
		return (printf("%s\n", pwd), free(pwd), 0);
	else
		return (1);
}
