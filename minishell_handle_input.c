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

static char *get_next_token(char **input, t_special_char_struct *special_char, int *position)
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
        special_char->type = (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE;
        special_char->count = 1;
        special_char->position = *position;
        start++;
        token = start;
        (*position)++;
        while (*start && *start != quote_char)
        {
            start++;
            (*position)++;
        }
        if (*start == quote_char)
        {
            special_char->count = 2;
            special_char->end_position = *position;
            *start = '\0';
            start++;
            (*position)++;
        }
        else
        {
            special_char->end_position = -1; // Unclosed quote
        }
    }
    else if (is_special_char(*start))
    {
        special_char->type = (e_special_char)*start;
        special_char->count = 1;
        special_char->position = *position;
        special_char->end_position = *position;
        start++;
        (*position)++;
        if ((special_char->type == REDIR_IN || special_char->type == REDIR_OUT) && *start == (char)special_char->type)
        {
            special_char->count = 2;
            special_char->end_position = *position;
            start++;
            (*position)++;
        }
    }
    else
    {
        while (*start && *start != ' ' && *start != '\t' && !is_special_char(*start))
        {
            start++;
            (*position)++;
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
    int special_char_index = 0;
    t_token *last_token = NULL;

    while (*input)
    {
        t_special_char_struct current_special_char = {END_OF_FILE, 0, -1, -1};
        token_start = get_next_token(&input, &current_special_char, &position);

        if (token_start)
        {
            t_token *new_token = malloc(sizeof(t_token));
            if (!new_token)
            {
                printf("Error: Failed to allocate memory for token\n");
                return 0;
            }
            init_token(new_token);  // Initialize the new token
            new_token->start = token_start;
            new_token->length = input - token_start;
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
        if (current_special_char.count > 0)
        {
            if (special_char_index >= parsed_input->special_char_count)
            {
                size_t new_size = parsed_input->special_char_count * 2;
                t_special_char_struct *new_special_char = realloc(parsed_input->special_char, 
                                                                  sizeof(t_special_char_struct) * new_size);
                if (!new_special_char)
                {
                    printf("Error: Failed to reallocate memory for special characters\n");
                    return 0;
                }
                parsed_input->special_char = new_special_char;
                parsed_input->special_char_count = new_size;
            }
            parsed_input->special_char[special_char_index] = current_special_char;
            special_char_index++;
        }
    }
    parsed_input->token_count = token_index;
    return 1;
}

static int allocate_initial_tokens(t_parsed_input *parsed_input)
{
    parsed_input->token_count = 20;  // Start with space for 10 tokens
    parsed_input->special_char_count = 20;  // Start with space for 10 special chars

    parsed_input->token = malloc(sizeof(char *) * parsed_input->token_count);
    parsed_input->special_char = calloc(parsed_input->special_char_count, sizeof(t_special_char_struct));
    if (!parsed_input->token || !parsed_input->special_char)
    {
        printf("Error: Failed to allocate initial memory for tokens or special characters\n");
        return 0;
    }

    return 1;
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;
    int i;

    init_parsed_input(&parsed_input);
    if (!allocate_initial_tokens(&parsed_input))
    {
        printf("Error: Failed to allocate initial tokens\n");
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
    for (i = 0; i < parsed_input.special_char_count && parsed_input.special_char[i].count > 0 && parsed_input.special_char != 0; i++)
    {
        printf("special char: %c count: %d position: %d end_position: %d\n",
               parsed_input.special_char[i].type,
               parsed_input.special_char[i].count,
               parsed_input.special_char[i].position,
               parsed_input.special_char[i].end_position);
    }
    free_parsed_input(&parsed_input);
    return 0;
}
