/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:28:41 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 14:36:18 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

/*
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
                if (*(equal_sign + 1) == '\0' && !(cmd->args[i + 1]))
                    value = strdup("");
                else if (*(equal_sign + 1) == '\0' && cmd->args[i + 1])
                    value = strdup(cmd->args[i + 1]);
                else
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
}*/void	export_print(t_env_var *current)
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

void	handle_env_var_with_value(t_command *cmd, int i, char *equal_sign)
{
	char	*key;
	char	*value;

	key = ft_strndup(cmd->args[i], equal_sign - cmd->args[i]);
	value = ft_strdup(equal_sign + 1);
	set_or_create_env_var(&(cmd->shell->env_vars), key, value);
	free(key);
	free(value);
}

int export_argument_check(t_command *cmd)
{
    int i;

    i = 0;
	if (!cmd)
		return (1);
    while (cmd->args[i])
    {
        if (!ft_isalpha(cmd->args[i][0]))
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

int	ft_export(t_command *cmd)
{
	t_env_var	*current;
	int			i;
	char		*equal_sign;

	i = 0;
	current = cmd->shell->env_vars;
	if (export_argument_check(cmd) == 1)
		return (1);
	if (cmd->args[0] == NULL)
		export_print(current);
	else
	{
		while (cmd->args[i])
		{
			equal_sign = ft_strchr(cmd->args[i], '=');
			if (equal_sign)
				handle_env_var_with_value(cmd, i, equal_sign);
			else
				set_or_create_env_var(&(cmd->shell->env_vars), cmd->args[i],
					"");
			i++;
		}
	}
	return (0);
}
