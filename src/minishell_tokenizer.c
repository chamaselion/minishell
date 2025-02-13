/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:08:48 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:08:48 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*finalizing_token_list(t_token *token_list)
{
	assign_token_role(token_list);
	pop_quotemark_tokens(&token_list);
	concatenate_tokens(&token_list);
	return (token_list);
}

static void	process_input_segment(const char **input,
				t_raw_list *raw_list, t_shell *shell)
{
	t_raw_token	*token;

	if (is_redirection((char *)*input) || is_pipe((char *)*input))
	{
		token = tokenize_pipe_redirection(input);
		if (token)
			append_raw_token(&raw_list->first, &raw_list->last, token);
		return ;
	}
	if (**input == '\'')
		single_quote_wrapper(input, raw_list);
	else if (**input == '"')
		double_quote_wrapper(input, raw_list, shell->env_vars, shell);
	else
	{
		token = handle_non_quote_segment(input, shell);
		if (token)
			append_raw_token(&raw_list->first, &raw_list->last, token);
	}
}

t_raw_token	*handle_input(char *input, t_shell *shell)
{
	t_raw_list	raw_list;

	raw_list = init_raw_list();
	while (*input)
	{
		input = skip_whitespace(input);
		if (!*input)
			break ;
		process_input_segment((const char **)&input, &raw_list, shell);
	}
	return (raw_list.first);
}
