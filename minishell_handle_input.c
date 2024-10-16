/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_handle_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:19:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/09 11:30:26 by mnaum            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void add_special_char(t_parsed_input *parsed_input, e_special_char type, int position)
{
    if (parsed_input->special_char_count >= parsed_input->special_char_capacity)
    {
        size_t new_size = parsed_input->special_char_capacity * 2;
        t_special_char_struct *new_special_char = realloc(parsed_input->special_char, 
                                                          sizeof(t_special_char_struct) * new_size);
        if (!new_special_char)
        {
            printf("Error: Failed to reallocate memory for special characters\n");
            return;
        }
        parsed_input->special_char = new_special_char;
        parsed_input->special_char_capacity = new_size;
    }
    
    parsed_input->special_char[parsed_input->special_char_count].type = type;
    parsed_input->special_char[parsed_input->special_char_count].position = position;
    parsed_input->special_char_count++;
}

static char *get_next_token(char **input, t_parsed_input *parsed_input, int *position)
{
    char *start = *input;
    char *token = start;
    char quote_char = '\0';

    // Skip leading spaces
    while (*start == ' ' || *start == '\t')
    {
        start++;
        (*position)++;
    }
    token = start;

    if (*start == '\'' || *start == '"')
    {
        quote_char = *start;
        add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
        start++;
        (*position)++;
        token = start;  // Set token start after the opening quote
        while (*start && *start != quote_char)
        {
            if (*start == '$' && quote_char == '"')
            {
                add_special_char(parsed_input, DOLLAR, *position);
            }
            start++;
            (*position)++;
        }
        if (*start == quote_char)
        {
            add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
            *start = '\0';  // Null-terminate the token here
            start++;
            (*position)++;
        }
    }
    else
    {
        while (*start && *start != ' ' && *start != '\t' && *start != '\'' && *start != '"')
        {
            if (*start == '$' || *start == '|' || *start == '<' || *start == '>')
            {
                if (*start == '<' && *(start + 1) == '<')
                {
                    add_special_char(parsed_input, REDIR_IN, *position);
                    add_special_char(parsed_input, REDIR_IN, *position + 1);
                    start += 2;
                    *position += 2;
                }
                else if (*start == '>' && *(start + 1) == '>')
                {
                    add_special_char(parsed_input, REDIR_OUT, *position);
                    add_special_char(parsed_input, REDIR_OUT, *position + 1);
                    start += 2;
                    *position += 2;
                }
                else
                {
                    add_special_char(parsed_input, (e_special_char)*start, *position);
                    start++;
                    (*position)++;
                }
            }
            else
            {
                start++;
                (*position)++;
            }
        }
    }
    *input = start;
    return (token != start) ? token : NULL;
}

static int tokenize_input(t_parsed_input *parsed_input, char *input)
{
    char *token_start;
    int position = 0;
    int token_index = 0;
    t_token *last_token = NULL;

    while (*input)
    {
        token_start = get_next_token(&input, parsed_input, &position);

        if (token_start)
        {
            t_token *new_token = malloc(sizeof(t_token));
            if (!new_token)
            {
                printf("Error: Failed to allocate memory for token\n");
                return 0;
            }
            init_token(new_token);
            new_token->start = token_start;
            new_token->length = input - token_start - (*input && (*(input-1) == '\'' || *(input-1) == '"') ? 1 : 0);
            if (last_token)
            {
                last_token->next = new_token;
                new_token->prev = last_token;
            }
            else
                parsed_input->token = new_token;

            last_token = new_token;
            token_index++;
        }
    }
    parsed_input->token_count = token_index;
    return 1;
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;
    int i;

    init_parsed_input(&parsed_input);
    parsed_input.special_char_capacity = 20;  // Initial capacity
    parsed_input.special_char = malloc(sizeof(t_special_char_struct) * parsed_input.special_char_capacity);
    if (!parsed_input.special_char)
    {
        printf("Error: Failed to allocate memory for special characters\n");
        return 1;
    }

    if (!tokenize_input(&parsed_input, input))
    {
        printf("Error: Failed to tokenize input\n");
        free_parsed_input(&parsed_input);
        return 1;
    }

    i = 0;
    for (t_token *current = parsed_input.token; current != NULL; current = current->next)
    {
        printf("Token %d: %.*s\n", i, current->length, current->start);
        i++;
    }
    for (i = 0; i < parsed_input.special_char_count; i++)
    {
        printf("special char: %c position: %d\n",
               parsed_input.special_char[i].type,
               parsed_input.special_char[i].position);
    }
    free_parsed_input(&parsed_input);
    return 0;
}
