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
	if (is_quote_char(*token->content))
	{
		token->role = ROLE_DELIMITER;
		token->command_expected = 0;
		if (token->next)
		{
			token->next->command_expected = 1;
			token->next->role = ROLE_EXECUTABLE;
			token = token->next;
		}
	}
	else
	{
		token->role = ROLE_EXECUTABLE;
		token->command_expected = 1;
	}
}

void	handle_quote_token(t_token *token)
{
	token->role = ROLE_DELIMITER;
}

void	handle_pipe_token(t_token *token)
{
	token->role = ROLE_PIPE;
	handle_first_token(token->next);
}

void	handle_redirect_token(t_token *token)
{
	token->role = ROLE_REDIRECT;
	token->command_expected = 0;
}

void	assign_token_role(t_token *token_list)
{
	t_token *current = token_list;

	// printf("Entered assign_token_role\n");
	if (current && !current->prev)
	{
		// printf("Handling first token: %s\n", current->content);
		handle_first_token(current);
		if (current->next)
		{
			current = current->next;
		}
		else
		{
			// printf("Only one token in the list.\n");
			return ;
		}
	}

	while (current)
	{
		// printf("Handling token: %s\n", current->content);
		if (is_quote_char(*current->content)
			&& current->quote_state == NO_QUOTE)
		{
			handle_quote_token(current);
		}
		else if (is_pipe(current->content))
		{
			handle_pipe_token(current);
		}
		else if (is_redirection(current->content))
		{
			handle_redirect_token(current);
		}
		else if (identify_env_var(current->content)
			&& current->quote_state == 0)
		{
			current->role = ROLE_VARIABLE;
		}
		else if (current->command_expected == 1)
		{
			current->role = ROLE_EXECUTABLE;
		}
		else
		{
			current->role = ROLE_ARGUMENT;
		}
		// printf("Role: %d, Content: %s\n", current->role, current->content);
		current = current->next;
	}
	// printf("Exited assign_token_role\n");
}

/* for debugging
void	print_token_list(t_token *token_list)
{
	t_token *current = token_list;
	while (current != NULL)
	{
		printf("Token: %s, Role: %d, Command Expected: %d, quote state: %d\n",
			current->content, current->role, current->command_expected,
			current->quote_state);
		current = current->next;
	}
}*/