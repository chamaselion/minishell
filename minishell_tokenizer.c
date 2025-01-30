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
	validate_token_syntax(token_list);
	add_space_tokens(token_list);
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

void add_space_tokens(t_token *tokens)
{
	t_token *current;
	t_token *next;
	t_token *space_token;

	current = tokens;
	while (current && current->next)
	{
		next = current->next;
		if (current->quote_state == NO_QUOTE &&
			next->quote_state == NO_QUOTE && 
			current->role == ROLE_ARGUMENT)
		{
			space_token = create_token(" ", ROLE_ARGUMENT);
			if (!space_token)
				return ;
			space_token->next = next;
			space_token->prev = current;
			current->next = space_token;
			next->prev = space_token;
			current = next;
		}
		else
			current = current->next;
	}
}

t_token	*create_token(const char *content, int role)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->content = ft_strdup(content);
	if (token->content == NULL)
		return (free(token), NULL);
	token->quote_state = NO_QUOTE;
	token->role = role;
	token->command_expected = 0;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}
