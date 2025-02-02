/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_roles_cmdexpected2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <mnaumann@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:16:43 by root              #+#    #+#             */
/*   Updated: 2025/02/02 16:23:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote_token(t_token *token)
{
	token->role = ROLE_DELIMITER;
}

void	handle_pipe_token(t_token *token)
{
	token->role = ROLE_PIPE;
	token->command_expected = 0;
	handle_first_token(token->next);
}

void	handle_redirect_token(t_token *token)
{
	token->role = ROLE_REDIRECT;
	token->command_expected = 0;
}
