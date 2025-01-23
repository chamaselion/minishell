/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_purging_quotemarks.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 09:35:22 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/22 18:05:19 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void check_for_unclosed(t_token *token_list)
{
    //printf("check entered\n");
    t_token *current = token_list;

    while (current) 
    {
        if (current->quote_state != 0 && !(is_quote_char(*current->content)))
        {
            //printf("Unclosed quote\n"); //implement error handling
			
            return;
        }
        current = current->next;
    }
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
    //printf("pop entered\n");
    while (current)
    {
        next = current->next;
        if (is_quote_char(*current->content) && current->quote_state == NO_QUOTE)
        {
            if (current == *token_list)
                *token_list = next;
            remove_token(token_list, current);
        }
        if (current->next == NULL)
            break ;
        current = next;
    }
	return(*token_list);
}
