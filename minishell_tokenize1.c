/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:19:01 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/06 15:51:37 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int refine_token(t_parsed_input *parsed_input, t_token *token)
{
    char    *pos;
    char    *start;

    if (token->quote_state != NO_QUOTE)
        return 1; // Don't refine tokens within quotes
    pos = token->start;
    start = pos;
    while (*pos)
    {
        if (is_special_char(*pos))
        {
            if (pos > start)
                add_token(parsed_input, start, pos - start);
            add_token(parsed_input, pos, 1);
            start = pos + 1;
        }
        pos++;
    }
    if (pos > start)
        add_token(parsed_input, start, pos - start);
    return 1;
}

int refine_tokens(t_parsed_input *parsed_input)
{
    t_token *current;
    t_token *next;

    current = parsed_input->token;
    while (current)
    {
        next = current->next;
        if (!refine_token(parsed_input, current))
            return (printf("crashed here"), 0);
        current = next;
    }
    return 1;
}

int add_token(t_parsed_input *parsed_input, char *start, int length)
{
    t_token *new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);
    init_token(new_token);
    new_token->start = ft_substr(start, 0, length);
    if (!new_token->start)
    {
        free(new_token);
        return (ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);
    }
    new_token->length = length;
    if (parsed_input->token)
    {
        parsed_input->last_token->next = new_token;
        new_token->prev = parsed_input->last_token;
    }
    else
        parsed_input->token = new_token;
    parsed_input->last_token = new_token;
    parsed_input->token_count++;
    return 1;
}