/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:50:41 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:06:22 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(t_env_var **env_vars, t_env_var *var)
{
	if (var->prev)
		var->prev->next = var->next;
	else
		*env_vars = var->next;
	if (var->next)
		var->next->prev = var->prev;
	free(var->string);
	free(var->key);
	free(var->value);
	free(var);
}

bool	is_valid_unset(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	unset_argument_check(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (1);
	while (cmd->args[i])
	{
		if (!is_valid_unset(cmd->args[i]))
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_unset(t_command *cmd)
{
	t_env_var	*current;
	int			i;

	if (!cmd->args[0])
		return (0);
	i = 0;
	if (unset_argument_check(cmd) == 1)
		return (1);
	while (cmd->args[i])
	{
		current = cmd->shell->env_vars;
		while (current)
		{
			if (ft_strcmp(current->key, cmd->args[i]) == 0)
			{
				remove_env_var(&cmd->shell->env_vars, current);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	return (0);
}
