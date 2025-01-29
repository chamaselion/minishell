/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:36:44 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/26 01:03:02 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_token	*finalizing_token_list(t_token *token_list)
{
	assign_token_role(token_list);
	pop_quotemark_tokens(&token_list);
	return (token_list);
}

t_raw_token	*handle_input(char *input, t_shell *shell)
{
	t_raw_list	raw_list;
	t_raw_token	*token;

	raw_list = init_raw_list();
	while (*input)
	{
		input = skip_whitespace(input);
		if (!*input)
			break ;
		if (is_redirection(input) || is_pipe(input))
		{
			token = tokenize_pipe_redirection((const char **)&input);
			if (token)
				append_raw_token(&raw_list.first, &raw_list.last, token);
			continue ;
		}
		if (*input == '\'')
			single_quote_wrapper((const char **)&input, &raw_list);
		else if (*input == '"')
			double_quote_wrapper((const char **)&input, &raw_list,
				shell->env_vars, shell);
		else
		{
			token = handle_non_quote_segment((const char **)&input, shell);
			if (token)
				append_raw_token(&raw_list.first, &raw_list.last, token);
		}
	}
	return (raw_list.first);
}

void	handle_redirect_token(t_token *token)
{
	token->role = ROLE_REDIRECT;
	token->command_expected = 0;
}
