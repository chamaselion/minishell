/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_roles_cmdexpected.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:08:28 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:08:28 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_first_quote(t_token *token)
{
	token->role = ROLE_DELIMITER;
	token->command_expected = 0;
	if (token->next)
	{
		token->next->command_expected = 1;
		token->next->separated = 1;
		token->next->role = ROLE_EXECUTABLE;
		token = token->next;
	}
}

void	handle_first_token(t_token *token)
{
	if (token && is_quote_char(*token->content))
		handle_first_quote(token);
	else if (token)
	{
		if (is_redirection(token->content))
		{
			handle_redirect_token(token);
		}
		else if (is_pipe(token->content))
			handle_pipe_token(token);
		else if (!is_pipe(token->content) && !is_redirection(token->content))
		{
			token->role = ROLE_EXECUTABLE;
			token->command_expected = 1;
			token->separated = 1;
		}
	}
	else
		return ;
}

static void	decide_token_role(t_token *current)
{
	if (is_quote_char(*current->content) && current->quote_state == NO_QUOTE)
		handle_quote_token(current);
	else if (is_pipe(current->content) && current->quote_state == 0)
		handle_pipe_token(current);
	else if (is_redirection(current->content) && current->quote_state == 0)
		handle_redirect_token(current);
	else if (identify_env_var(current->content) && current->quote_state == 0)
		current->role = ROLE_VARIABLE;
	else if (current->command_expected == 1)
		current->role = ROLE_EXECUTABLE;
	else
		current->role = ROLE_ARGUMENT;
}

void	assign_token_role(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (current && !current->prev)
	{
		handle_first_token(current);
		if (current->next)
			current = current->next;
		else
			return ;
	}
	while (current)
	{
		decide_token_role(current);
		current = current->next;
	}
}
