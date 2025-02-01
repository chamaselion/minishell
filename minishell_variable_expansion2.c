/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_expansion2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:48:09 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/29 19:13:33 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar_sign(const char *str, int *idx,
			char *output_str, t_shell *shell)
{
	char	*temp_char;

	if (str[*idx + 1] == '?')
	{
		temp_char = get_exit_code_str(shell);
		output_str = ft_strjoin_and_free2(output_str, temp_char);
		*idx += 2;
	}
	else if (ft_isalnum(str[*idx + 1]) || str[*idx + 1] == '_')
	{
		temp_char = resolve_variable(str, idx, shell);
		output_str = ft_strjoin_and_free2(output_str, temp_char);
	}
	else
	{
		temp_char = ft_strndup(&str[*idx], 1);
		output_str = ft_strjoin_and_free2(output_str, temp_char);
		(*idx)++;
	}
	return (output_str);
}

char	*resolve_variables_str(char *str, t_shell *shell)
{
	char			*output_str;
	unsigned char	quote_mode;
	int				idx;
	char			*temp_char;

	output_str = ft_strdup("");
	if (!output_str)
		return (NULL);
	idx = 0;
	quote_mode = 0;
	while (str[idx] != 0)
	{
		update_quote_mode(str[idx], &quote_mode);
		if (str[idx] == '$' && (!quote_mode || (quote_mode & 2)))
		{
			output_str = handle_dollar_sign(str, &idx, output_str, shell);
			if(*output_str == '\0')
				update_exit_code(shell, 0);
		}
		else
		{
			temp_char = ft_strndup(&str[idx], 1);
			output_str = ft_strjoin_and_free2(output_str, temp_char);
			idx++;
		}
	}
	return (output_str);
}

char	*resolve_variable(const char *str, int *idx, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*output_str;
	char	*temp_char;

	var_name = ft_strdup("");
	if (!var_name)
		return (NULL);
	(*idx)++;
	while (ft_isalnum(str[*idx]) || str[*idx] == '_')
	{
		temp_char = ft_strndup(&str[*idx], 1);
		var_name = ft_strjoin_and_free2(var_name, temp_char);
		(*idx)++;
	}
	var_value = ft_getenv(shell->env_vars, var_name);
	if (!var_value)
		var_value = "";
	output_str = ft_strdup(var_value);
	free(var_name);
	return (output_str);
}

void	update_quote_mode(char c, unsigned char *quote_mode)
{
	if (c == '\'' && !(*quote_mode))
		*quote_mode ^= 1;
	else if (c == '"' && !(*quote_mode))
		*quote_mode ^= 2;
}
