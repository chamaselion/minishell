/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:19:01 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/12 10:48:23 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int split_token_by_special_chars(t_parsed_input *parsed_input, t_token *token)
{
    char *pos = token->start;
    char *start = pos;
    t_token *prev = token->prev;

    while (*pos)
    {
        if (is_special_char(*pos))
        {
            if (pos > start)
            {
                if (!add_token_segment(parsed_input, ft_substr(start, 0, pos - start), prev, NO_QUOTE))
                    return 0;
                prev = prev->next;
            }
            if (!add_token_segment(parsed_input, ft_substr(pos, 0, 1), prev, NO_QUOTE))
                return 0;
            prev = prev->next;
            start = pos + 1;
        }
        pos++;
    }
    if (pos > start)
    {
        if (!add_token_segment(parsed_input, ft_substr(start, 0, pos - start), prev, NO_QUOTE))
            return 0;
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

static int refine_token(t_parsed_input *parsed_input, t_token **token_ptr)
{
    t_token *token;
    
    token = *token_ptr;
    if (token->quote_state != NO_QUOTE)
        return 1;

    if (!split_token_by_special_chars(parsed_input, token))
        return 0;

    remove_and_update_token(token_ptr, parsed_input, token);
    return 1;
}

int refine_tokens(t_parsed_input *parsed_input)
{
    t_token *current = parsed_input->token;
    t_token *next;

    while (current)
    {
        next = current->next;
        if (!refine_token(parsed_input, &current))
            return 0;
        current = next;
    }
    return 1;
}

int process_variables(t_parsed_input *parsed_input)
{
    t_token *current = parsed_input->token;

    while (current)
    {
        if (current->quote_state == WITHIN_DOUBLE_QUOTE)
        {
            if (!expand_variables_in_token(parsed_input, current))
                return 0;
        }
        current = current->next;
    }
    return 1;
}

int process_quoted_content(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state)
{
    if (quote_state == WITHIN_DOUBLE_QUOTE)
    {
        if (!split_and_add_token(parsed_input, content, prev, quote_state))
            return 0;
    }
    else
    {
        if (!add_segment(parsed_input, content, prev, quote_state))
            return 0;
    }
    return 1;
}

int expand_variables_in_token(t_parsed_input *parsed_input, t_token *token)
{
    char    *pos;
    char    *start;
    t_token *prev;

    pos = token->start;
    start = pos;
    prev = token->prev;
    while (*pos)
    {
        if (*pos == '$')
        {
            if (pos > start && !add_subtoken(parsed_input, &prev, start, pos - start, WITHIN_DOUBLE_QUOTE))
                return 0;
            start = pos++;
            while (*pos && (ft_isalnum(*pos) || *pos == '_'))
                pos++;
            if (!add_subtoken(parsed_input, &prev, start, pos - start, NO_QUOTE))
                return 0;
            start = pos;
        }
        else
            pos++;
    }
    if (pos > start && !add_subtoken(parsed_input, &prev, start, pos - start, WITHIN_DOUBLE_QUOTE))
        return 0;
    remove_token(parsed_input, token);
    return 1;
}

int add_subtoken(t_parsed_input *parsed_input, t_token **prev, char *start, int length, int quote_state)
{
    t_token *new_token;
    char    *content;

    content = ft_substr(start, 0, length);
    if (!content)
        return 0;
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (free(content), 0);
    init_token(new_token);
    new_token->start = content;
    new_token->length = length;
    new_token->quote_state = quote_state;
    new_token->role = ROLE_DEFAULT;
    insert_token_after(parsed_input, *prev, new_token);
    *prev = new_token;
    return 1;
}

void insert_token_after(t_parsed_input *parsed_input, t_token *prev, t_token *new_token)
{
    if (prev)
    {
        new_token->next = prev->next;
        if (prev->next)
            prev->next->prev = new_token;
        prev->next = new_token;
        new_token->prev = prev;
    }
    else
    {
        new_token->next = parsed_input->token;
        if (parsed_input->token)
            parsed_input->token->prev = new_token;
        parsed_input->token = new_token;
    }
}
