/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:36:44 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/24 11:44:29 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*finalizing_token_list(t_token *token_list)
{
	assign_token_role(token_list);
	check_for_unclosed(token_list);
	pop_quotemark_tokens(&token_list);
	return (token_list);
}

/*char* resolve_variables_str(char *str, t_shell *shell) {
	char *output_str = ft_strdup("");
	unsigned char quote_mode;
	int idx;

	idx = 0;
	quote_mode = 0; //01b Single quote, 10b Double quote
	while(str[idx] != 0) {
		if(str[idx] == '\'' && !quote_mode) {
			quote_mode ^= 1;
		}
		else if(str[idx] == '"' && !quote_mode) {
			quote_mode ^= 2;
		}
		
		if(str[idx] == '$' && (!quote_mode || quote_mode & 2)) {
			if(str[idx + 1] == '?') {
				output_str = ft_strjoin(output_str, get_exit_code_str(shell));
				idx += 2;
			} else {
				char *var_name = ft_strdup("");
				idx++;
				while(ft_isalnum(str[idx]) || str[idx] == '_') {
					var_name = ft_strjoin_and_free2(var_name, ft_strndup(&str[idx], 1));
					idx++;
				}
				
				char *var_value = ft_strdup(get_env_var(var_name, shell->env_vars));
				if(var_value)
					output_str = ft_strjoin_and_free2(output_str, var_value);
				free(var_name);
			}
		} else {
			output_str = ft_strjoin_and_free2(output_str, ft_strndup(&str[idx], 1));
			idx++;
		}		
	}
	output_str = ft_strjoin_and_free(output_str, "\0");
	return output_str;
}*/

char	*resolve_variables_str(char *str, t_shell *shell)
{
	char			*output_str;
	unsigned char	quote_mode;
	int				idx;
	char			*var_name;
	char			*var_value;

	output_str = ft_strdup("");
	idx = 0;
	quote_mode = 0; // 01b Single quote, 10b Double quote
	while (str[idx] != 0)
	{
		if (str[idx] == '\'' && !quote_mode)
		{
			quote_mode ^= 1;
		}
		else if (str[idx] == '"' && !quote_mode)
		{
			quote_mode ^= 2;
		}
		if (str[idx] == '$' && (!quote_mode || quote_mode & 2))
		{
			if (str[idx + 1] == '?')
			{
				output_str = ft_strjoin_and_free2(output_str,
						get_exit_code_str(shell));
				idx += 2; // Increment idx by 2 to skip both $ and ?
			}
			else if (ft_isalnum(str[idx + 1]) || str[idx + 1] == '_')
			{
				var_name = ft_strdup("");
				idx++;
				while (ft_isalnum(str[idx]) || str[idx] == '_')
				{
					var_name = ft_strjoin_and_free2(var_name,
							ft_strndup(&str[idx], 1));
					idx++;
				}
				var_value = ft_getenv(shell->env_vars, var_name);
				if (var_value)
				{
					output_str = ft_strjoin_and_free2(output_str, var_value);
				}
				free(var_name); // Free the allocated memory for var_name
			}
			else
			{
				// Handle case where $ is followed by a non-alphanumeric character or is at the end of the string
				output_str = ft_strjoin_and_free2(output_str,
						ft_strndup(&str[idx], 1));
				idx++;
			}
		}
		else
		{
			output_str = ft_strjoin_and_free2(output_str, ft_strndup(&str[idx],
						1));
			idx++;
		}
	}
	output_str = ft_strjoin_and_free(output_str, "\0");
	return (output_str);
}

t_raw_token	*handle_input(char *input, t_shell *shell)
{
	t_raw_token	*first;
	t_raw_token	*last;
	t_raw_token	*token;
	int			pos;

	first = NULL;
	last = NULL;
	pos = 0;
	input = resolve_variables_str(input, shell);
	// printf("EDITED STRING: %s\n", input);
	while (*input != '\0')
	{
		input = skip_whitespace(input);
		pos++;
		if (*input == '\'')
			single_quote_wrapper((const char **)&input, &pos, &first, &last);
		else if (*input == '"')
			double_quote_wrapper((const char **)&input, &pos, &first, &last, shell->env_vars);
		else
		{
			token = handle_non_quote_segment((const char **)&input, &pos);
			append_raw_token(&first, &last, token);
		}
	}
	return (first);
}
