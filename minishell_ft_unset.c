/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:50:41 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/21 14:57:40 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

static void remove_env_var(t_env_var **env_vars, t_env_var *var)
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

int ft_unset(t_command *cmd)
{
    t_env_var *current;
    int i;

    if (!cmd->args[0])
        return (1);
    i = 0;
    while (cmd->args[i])
    {
        current = cmd->shell->env_vars;
        while (current)
        {
            if (strcmp(current->key, cmd->args[i]) == 0)
            {
                remove_env_var(&cmd->shell->env_vars, current);
                break;
            }
            current = current->next;
        }
        i++;
    }
	return 0;
}
