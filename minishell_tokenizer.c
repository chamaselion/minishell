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

void concatenate_tokens(t_token **token_list)
{
	t_token *current;
	t_token *next;
	char    *new_content;
	int     did_concat;

	if (!token_list || !*token_list)
		return;
	do
	{
		did_concat = 0;
		current = *token_list;
		while (current && current->next)
		{
			next = current->next;
			if (current->role == ROLE_ARGUMENT && 
				!current->separated && 
				!is_quote_char(*current->content) &&
				!is_quote_char(*next->content) &&
				(current->quote_state != NO_QUOTE || 
				next->quote_state != NO_QUOTE))
			{
				new_content = ft_strjoin(current->content, next->content);
				if (!new_content)
					return;
				current->content = new_content;
				if (next->quote_state != NO_QUOTE)
					current->separated = next->separated;
				current->next = next->next;
				if (next->next)
					next->next->prev = current;
				free(next->content);
				free(next);
				did_concat = 1;
			}
			else
				current = current->next;
		}
	} while (did_concat);
}

t_token *finalizing_token_list(t_token *token_list)
{
	assign_token_role(token_list);
	pop_quotemark_tokens(&token_list);
	concatenate_tokens(&token_list);
	//validate_token_syntax(token_list);
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

