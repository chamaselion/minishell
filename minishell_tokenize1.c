/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:19:01 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/02 16:26:22 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *process_quoted_token(char **input, t_parsed_input *parsed_input,
    int *position, t_token **last_token)
{
    char quote_char;
    char *start;
    char *token;

    start = *input;
    quote_char = *start;
    add_token_to_list(parsed_input, start, 1, last_token);
    add_special_char(parsed_input, get_quote_type(quote_char), *position);
    return (process_quote_content(input, parsed_input, position, last_token));
}

static char *process_quote_content(char **input, t_parsed_input *parsed_input,
    int *position, t_token **last_token)
{
    char *start;
    char *token;

    (*input)++;
    (*position)++;
    start = *input;
    token = start;
    while (**input && **input != quote_char)
    {
        if (process_dollar_in_quotes(input, parsed_input, position,
            last_token) == 0)
            return (NULL);
        (*input)++;
        (*position)++;
    }
    return (finalize_quote_processing(input, parsed_input, position,
        last_token, token));
}

static int process_dollar_in_quotes(char **input, t_parsed_input *parsed_input,
    int *position, t_token **last_token)
{
    if (**input == '$' && quote_char == '"')
    {
        if (!handle_dollar_token(input, parsed_input, position,
            last_token, token))
            return (0);
        token = *input;
    }
    return (1);
}
