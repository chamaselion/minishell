/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:04:11 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/23 23:04:12 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = malloc(sizeof(char) * (32768 + 1));
	if (getcwd(pwd, 32768))
		return (printf("%s\n", pwd), 0);
	else
		return (1);
}
