/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:28:41 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/21 14:58:34 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int ft_export(t_command *cmd)
{
    t_env_var *current;
    int i;
    char *key;
    char *value;
    char *equal_sign;

	i = 0;
    current = cmd->shell->env_vars;

	if (!cmd)
		return (1);
    if (cmd->args[0] == NULL)
    {
        while (current)
        {
            if (current->value)
                printf("declare -x %s=\"%s\"\n", current->key, current->value);
            else
                printf("declare -x %s\n", current->key);
            current = current->next;
        }
    }
    else
    {
        while (cmd->args[i])
        {
            equal_sign = strchr(cmd->args[i], '=');
            if (equal_sign)
            {
                key = strndup(cmd->args[i], equal_sign - cmd->args[i]);
                value = strdup(equal_sign + 1);
                set_or_create_env_var(&(cmd->shell->env_vars), key, value);
                free(key);
                free(value);
            }
            else
            {
                set_or_create_env_var(&(cmd->shell->env_vars), cmd->args[i], "");
            }
			i++;
        }
    }
	return (0);
}
