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

t_token	*finalizing_token_list(t_token *token_list, t_shell *shell)
{
	assign_token_role(token_list);
	if (check_for_unclosed(token_list, shell) == 1)
		return (NULL);
	pop_quotemark_tokens(&token_list);
	return (token_list);
}

t_raw_token	*handle_input(char *input, t_shell *shell)
{
	t_raw_list	raw_list;
	t_raw_token	*token;

	raw_list = init_raw_list();
	input = skip_whitespace(input);
	while (*input != '\0')
	{
		input = skip_whitespace(input);
		if (*input == '\'')
			single_quote_wrapper((const char **)&input, &raw_list);
		else if (*input == '"')
			double_quote_wrapper((const char **)&input, &raw_list,
				shell->env_vars);
		else
		{
			token = handle_non_quote_segment((const char **)&input, shell);
			append_raw_token(&raw_list.first, &raw_list.last, token);
		}
	}
	return (raw_list.first);
}
