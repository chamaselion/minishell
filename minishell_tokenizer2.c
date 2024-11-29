/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:12:59 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/29 11:46:22 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *assign_role(t_raw_token raw)
{
	t_token *token = malloc(sizeof(t_token));
	init_token(token);
	token->content = ft_strdup(raw.segment);
	token->quote_state = raw.quote_state;
	token->position = raw.position;
	if (is_redirection(raw.segment))
		token->role = ROLE_REDIRECT;
	else if (is_quote_char(*raw.segment))
		token->role = ROLE_DELIMITER;
	else
		token->role = ROLE_ARGUMENT;
	return token;
}