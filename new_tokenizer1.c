/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokenizer1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:28:18 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/12 12:11:56 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int initial_tokenization(t_parsed_input *parsed_input, char *input)
{
    int     i;
    int     start;
    t_quote_state quote_state;

    i = 0;
    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        start = i;
        quote_state = NO_QUOTE;
        while (input[i])
        {
            if (input[i] == '\'' && quote_state == NO_QUOTE)
                quote_state = WITHIN_SINGLE_QUOTE;
            else if (input[i] == '\'' && quote_state == WITHIN_SINGLE_QUOTE)
                quote_state = NO_QUOTE;
            else if (input[i] == '\"' && quote_state == NO_QUOTE)
                quote_state = WITHIN_DOUBLE_QUOTE;
            else if (input[i] == '\"' && quote_state == WITHIN_DOUBLE_QUOTE)
                quote_state = NO_QUOTE;
            else if ((input[i] == ' ' || input[i] == '\t') && quote_state == NO_QUOTE)
                break;
            i++;
        }
        if (i > start && !add_token(parsed_input, &input[start], i - start, quote_state))
			return 0;
    }
    return 1;
}

int add_token(t_parsed_input *parsed_input, char *start, int length, t_quote_state quote_state)
{
    t_token *new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);

    new_token->start = ft_substr(start, 0, length);
    if (!new_token->start)
        return (free(new_token), ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);

    new_token->length = length;
    new_token->quote_state = quote_state;
    new_token->role = ROLE_DEFAULT;
    new_token->prev = parsed_input->last_token;
    new_token->next = NULL;

    if (parsed_input->last_token)
        parsed_input->last_token->next = new_token;
    else
        parsed_input->token = new_token;
    parsed_input->last_token = new_token;
    parsed_input->token_count++;
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
        if (current->quote_state == WITHIN_DOUBLE_QUOTE)
        {
            if (!process_double_quote_token(parsed_input, current))
                return 0;
        }
        else if (current->quote_state == NO_QUOTE)
        {
            if (!process_no_quote_token(parsed_input, current))
                return 0;
        }
        current = next;
    }
    return 1;
}

int process_double_quote_token(t_parsed_input *parsed_input, t_token *token)
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

int process_no_quote_token(t_parsed_input *parsed_input, t_token *token)
{
    char    *pos;
    char    *start;
    t_token *prev;

    pos = token->start;
    start = pos;
    prev = token->prev;
    while (*pos)
    {
        if (is_special_char(*pos))
        {
            if (pos > start && !add_subtoken(parsed_input, &prev, start, pos - start, NO_QUOTE))
                return 0;
            start = pos;
            pos++;
            if (!add_subtoken(parsed_input, &prev, start, pos - start, NO_QUOTE))
                return 0;
            start = pos;
        }
        else
            pos++;
    }
    if (pos > start && !add_subtoken(parsed_input, &prev, start, pos - start, NO_QUOTE))
        return 0;
    remove_token(parsed_input, token);
    return 1;
}

int add_subtoken(t_parsed_input *parsed_input, t_token **prev, char *start, int length, int quote_state)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);

	new_token->start = ft_substr(start, 0, length);
	if (!new_token->start)
		return (free(new_token), ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);

	new_token->length = length;
	new_token->quote_state = quote_state;
	new_token->role = ROLE_DEFAULT;
	new_token->prev = *prev;
	new_token->next = NULL;

	if (*prev)
		(*prev)->next = new_token;
	else
		parsed_input->token = new_token;
	*prev = new_token;
	parsed_input->last_token = new_token;
	parsed_input->token_count++;
	return 1;
}

void remove_token(t_parsed_input *parsed_input, t_token *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		parsed_input->token = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free(token->start);
	free(token);
}
