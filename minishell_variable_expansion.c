/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_expansion.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:17:27 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/10 13:22:02 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*extract_variable_name(const char *current, char *var_name)
{
	int	var_idx;

	var_idx = 0;
	while (*current != '\0' && (ft_isalnum(*current) || *current == '_'))
	{
		var_name[var_idx] = *current;
		var_idx++;
		current++;
	}
	var_name[var_idx] = '\0';
	return (current);
}

void	append_variable_value(const char *var_name,
			char **write_ptr, t_env_var *env)
{
	char	*env_value;

	if (is_valid_env_var_name(var_name))
	{
		env_value = ft_getenv(env, var_name);
		if (env_value != NULL)
		{
			ft_strcpy(*write_ptr, env_value);
			*write_ptr += ft_strlen(env_value);
		}
	}
	else
	{
		**write_ptr = '$';
		(*write_ptr)++;
		ft_strcpy(*write_ptr, var_name);
		*write_ptr += ft_strlen(var_name);
	}
}

void	append_character(char c, char **write_ptr)
{
	**write_ptr = c;
	(*write_ptr)++;
}

char	*expand_double_quote_content(const char *content, t_env_var *env)
{
	char		buffer[1024];
	char		var_name[256];
	char		*write_ptr;
	const char	*current;

	ft_memset(buffer, 0, sizeof(buffer));
	write_ptr = buffer;
	current = content;
	while (*current != '\0')
	{
		if (*current == '$')
		{
			current++;
			current = extract_variable_name(current, var_name);
			append_variable_value(var_name, &write_ptr, env);
		}
		else
		{
			append_character(*current, &write_ptr);
			current++;
		}
	}
	*write_ptr = '\0';
	return (ft_strdup(buffer));
}

int	is_valid_env_var_name(const char *str)
{
	const char	*current;

	if (str == NULL || *str == '\0')
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	current = str + 1;
	while (*current != '\0')
	{
		if (!ft_isalnum(*current) && *current != '_')
			return (0);
		current++;
	}
	return (1);
}
