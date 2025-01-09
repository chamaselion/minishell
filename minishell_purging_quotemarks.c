/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_purging_quotemarks.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 09:35:22 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/19 11:02:12 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_for_unclosed(t_token *token_list)
{
	while (token_list->next)
		token_list = token_list->next;
	if (token_list->prev->quote_state != 0 && !(is_quote_char(*token_list->content)))
		return ; //implement error handling "unclosed quotes" (in my opinion just a subcase of syntax error)
}

void remove_token(t_token **head, t_token *token)
{
    if (token->prev)
        token->prev->next = token->next;
    if (token->next)
        token->next->prev = token->prev;
    if (*head == token)
        *head = token->next;
    free(token->content);
    free(token);
}

t_token *pop_quotemark_tokens(t_token **token_list)
{
    t_token *current;
    t_token *next;

    current = *token_list;
    while (current)
    {
        next = current->next;
        if (is_quote_char(*current->content) && current->quote_state == NO_QUOTE)
        {
            if (current == *token_list)
                *token_list = next;
            remove_token(token_list, current);
        }
        current = next;
    }
	return(*token_list);
}
