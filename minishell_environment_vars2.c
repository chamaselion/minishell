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
		const char *value)
{
	t_env_var	*current;

	current = *env_vars;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_env_var(current, key, value);
			current->format = 1;
			return ;
		}
		current = current->next;
	}
	create_new_env_var(env_vars, key, value);
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

void	create_new_env_var_hidden(t_env_var **env_vars, const char *key)
{
    t_env_var	*new_var;
    size_t		len;

    new_var = (t_env_var *)malloc(sizeof(t_env_var));
    if (!new_var)
        return ;
    new_var->key = ft_strdup(key);
    new_var->value = ft_strdup("");
    len = ft_strlen(new_var->key) + ft_strlen(new_var->value) + 2;
    new_var->string = (char *)malloc(len);
	if (!new_var->string)
		return ;
    ft_strlcpy(new_var->string, new_var->key, len);
    ft_strlcat(new_var->string, "=", len);
    ft_strlcat(new_var->string, new_var->value, len);
    new_var->length = len - 1;
    new_var->format = 0; 
    new_var->next = *env_vars;
    new_var->prev = NULL;
    if (*env_vars)
        (*env_vars)->prev = new_var;
    *env_vars = new_var;
}

void	set_or_create_hidden_env_var(t_env_var **env_vars, const char *key)
{
	t_env_var	*current;

	current = *env_vars;
	while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return ;
        current = current->next;
    }
    create_new_env_var_hidden(env_vars, key);
}