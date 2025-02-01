/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_environment_vars.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:48:20 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 14:57:36 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_var	*create_env_var(const char *env_str)
{
	t_env_var	*env_var;
	char		*equal_sign;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
	{
		env_var->key = ft_strndup(env_str, equal_sign - env_str);
		env_var->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		env_var->key = ft_strdup(env_str);
		env_var->value = NULL;
	}
	env_var->string = ft_strdup(env_str);
	env_var->length = ft_strlen(env_str);
	env_var->format = (equal_sign != NULL);
	env_var->next = NULL;
	env_var->prev = NULL;
	return (env_var);
}

void	add_env_var(t_env_var **head, t_env_var *new_var)
{
	t_env_var	*current;

	if (!*head)
	{
		*head = new_var;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_var;
	new_var->prev = current;
}

t_env_var	*init_env_vars(char **envp)
{
	t_env_var	*env_vars;
	t_env_var	*new_var;
	int			i;

	i = 0;
	env_vars = NULL;
	while (envp[i])
	{
		new_var = create_env_var(envp[i]);
		if (new_var)
			add_env_var(&env_vars, new_var);
		i++;
	}
	return (env_vars);
}

void	update_env_var(t_env_var *current, const char *key,
		const char *value)
{
	size_t	len;

	free(current->value);
	current->value = ft_strdup(value);
	free(current->string);
	len = ft_strlen(key) + ft_strlen(value) + 2;
	current->string = (char *)malloc(len);
	ft_strlcpy(current->string, key, len);
	ft_strlcat(current->string, "=", len);
	ft_strlcat(current->string, value, len);
	current->length = len - 1;
}

void	create_new_env_var(t_env_var **env_vars, const char *key,
		const char *value)
{
	t_env_var	*new_var;
	size_t		len;

	new_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!new_var)
		return ;
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	len = ft_strlen(key) + ft_strlen(value) + 2;
	new_var->string = (char *)malloc(len);
	ft_strlcpy(new_var->string, key, len);
	ft_strlcat(new_var->string, "=", len);
	ft_strlcat(new_var->string, value, len);
	new_var->length = len - 1;
	new_var->format = 1;
	new_var->next = *env_vars;
	new_var->prev = NULL;
	if (*env_vars)
		(*env_vars)->prev = new_var;
	*env_vars = new_var;
}
