/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:02:05 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 00:42:13 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_command *cmd)
{
	t_env_var	*current;

	current = cmd->shell->env_vars;
	if (!cmd)
		return (1);
	if (cmd->arg_count > 0)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (2);
	}
	if (cmd->args[0] == NULL)
	{
		while (current)
		{
			if (current->value && current->format)
				printf("%s=%s\n", current->key, current->value);
			else if (current->format)
				printf("%s\n", current->key);
			current = current->next;
		}
	}
	return (0);
}
