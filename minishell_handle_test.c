/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_handle_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:10 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/06 14:05:23 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int initial_tokenization(t_parsed_input *parsed_input, char *input)
{
    char *start;
    int   quote_state;

    while (*input)
    {
        skip_whitespace(&input);
        if (*input == '\0')
            break;
        start = input;
        quote_state = NO_QUOTE;
        while (*input && (!is_delimiter(*input) || quote_state != NO_QUOTE))
        {
            if (is_quote_char(*input))
                quote_state = toggle_quote_state(*input, quote_state);
            input++;
        }
        if (input > start)
        {
            if (!add_token(parsed_input, start, input - start))
                return 0;
        }
    }
    return 1;
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;

    init_parsed_input(&parsed_input);
    if (!initial_tokenization(&parsed_input, input))
        return (free_tokens(parsed_input.token), 1);
    print_tokens(&parsed_input);
	if (!process_quotes(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    print_tokens(&parsed_input);
    if (!check_unclosed_quotes(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    print_tokens(&parsed_input);
    printf("Before refine_tokens\n");
    if (!refine_tokens(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    printf("After refine_tokens\n");
    print_tokens(&parsed_input);
    if (!assign_roles(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    // execute
    print_tokens(&parsed_input);
    free_tokens(parsed_input.token);
    return 0;
}

void free_tokens(t_token *token)
{
    t_token *tmp;

    while (token)
    {
        tmp = token;
        token = token->next;
        free(tmp->start);
        free(tmp);
    }
}

void print_tokens(t_parsed_input *parsed_input)
{
    t_token *current = parsed_input->token;
    int i = 0;
    while (current)
    {
        printf("Token %d: '%s', Role: %d\n", i++, current->start, current->role);
        current = current->next;
    }
}

void skip_whitespace(char **input)
{
    while (**input == ' ' || **input == '\t')
        (*input)++;
}

