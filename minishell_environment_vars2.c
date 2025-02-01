/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_environment_vars2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:55:28 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 15:00:36 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_or_create_env_var(t_env_var **env_vars, const char *key,
		const char *value, int has_equal)
{
	t_env_var	*current;

	current = *env_vars;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_env_var(current, key, value);
			return ;
		}
		current = current->next;
	}
	create_new_env_var(env_vars, key, value, has_equal);
}

char	*ft_getenv(t_env_var *env_vars, const char *name)
{
	t_env_var	*current;

	if (!env_vars || !name)
		return (NULL);
	current = env_vars;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
