/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:52:31 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/12 10:58:47 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_quote_state(char c)
{
    if (c == '\'')
        return WITHIN_SINGLE_QUOTE;
    else
        return WITHIN_DOUBLE_QUOTE;
}

int is_matching_quote(char c, int state)
{
    return ((c == '\'' && state == WITHIN_SINGLE_QUOTE) ||
            (c == '\"' && state == WITHIN_DOUBLE_QUOTE));
}

int add_segment(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state)
{
    if (!add_token_segment(parsed_input, content, *prev, quote_state))
        return 0;
    *prev = (*prev)->next;
    return 1;
}

void remove_and_update_token(t_token **token_ptr, t_parsed_input *parsed_input, t_token *token)
{
    remove_token(parsed_input, token);
    if (token->prev)
        *token_ptr = token->prev->next;
    else
        *token_ptr = parsed_input->token;
}

int is_redirection(char *str)
{
    if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0 ||
        ft_strcmp(str, "<<") == 0 || ft_strcmp(str, ">>") == 0)
        return 1;
    return 0;
}