/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_roles_cmdexpected.c                      :+:      :+:    :+:   */
/*                                                    +:+
				+:+			+:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+		+#+       */
/*                                                +#+#+#+#+#	+#+           */
/*   Created: 2024/12/03 18:28:30 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/04 18:39:47 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_first_token(t_token *token)
{
	if (token && is_quote_char(*token->content))
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
	else if (token)
	{
		token->role = ROLE_EXECUTABLE;
		token->command_expected = 1;
		token->separated = 1;
	}
	else
		return ;
}

/*
void	handle_first_token(t_token *token)
{
	if (!token)
		return ;
	if (ft_strcmp(token->content, "|") == 0)
	{
		token->role = ROLE_PIPE;
		token->command_expected = 0;
		token->separated = 1;
		if (token->next)
		{
			token->next->command_expected = 1;
			token->next->separated = 1;
			token->next->role = ROLE_EXECUTABLE;
		}
	}
	else if (ft_strcmp(token->content, ">") == 0 || ft_strcmp(token->content,
			"<") == 0 || ft_strcmp(token->content, ">>") == 0
		|| ft_strcmp(token->content, "<<") == 0)
	{
		token->role = ROLE_REDIRECT;
		token->command_expected = 0;
		if (token->next)
		{
			token->next->command_expected = 1;
			token->next->separated = 1;
		}
	}
	else if (is_quote_char(*token->content))
	{
		token->role = ROLE_DELIMITER;
		token->command_expected = 0;
		if (token->next)
		{
			token->next->command_expected = 1;
			token->next->separated = 1;
			token->next->role = ROLE_EXECUTABLE;
		}
	}
	else
	{
		token->role = ROLE_EXECUTABLE;
		token->command_expected = 1;
		token->separated = 1;
	}
}
*/
void	handle_quote_token(t_token *token)
{
	token->role = ROLE_DELIMITER;
}

void	handle_pipe_token(t_token *token)
{
	token->role = ROLE_PIPE;
	handle_first_token(token->next);
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
	t_token *current;

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
