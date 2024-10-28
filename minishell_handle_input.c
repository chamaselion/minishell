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
    parsed_input->special_char[parsed_input->special_char_count].type = type;
    parsed_input->special_char[parsed_input->special_char_count].position = position;
    parsed_input->special_char_count++;
}

static void add_token_to_list(t_parsed_input *parsed_input, char *token_start, 
                            int length, t_token **last_token)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        printf("Error: Failed to allocate memory for token\n");
        return;
    }
    init_token(new_token);

    // Allocate memory for the token content and copy it
    new_token->start = malloc(length + 1);
    if (!new_token->start)
    {
        printf("Error: Failed to allocate memory for token content\n");
        free(new_token);
        return;
    }
    strncpy(new_token->start, token_start, length);
    new_token->start[length] = '\0';
    new_token->length = length;

    // Link the token in the doubly linked list
    if (*last_token)
    {
        (*last_token)->next = new_token;
        new_token->prev = *last_token;
    }
    else
        parsed_input->token = new_token;

    *last_token = new_token;
}

static char *get_next_token(char **input, t_parsed_input *parsed_input, 
                          int *position, t_token **last_token)
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
        // Add quote as a token
        add_token_to_list(parsed_input, start, 1, last_token);
        add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
        
        start++;
        (*position)++;
        token = start;

        while (*start && *start != quote_char)
        {
            if (*start == '$' && quote_char == '"')
            {
                // Add text before the $ as a token if it exists
                if (start > token)
                {
                    add_token_to_list(parsed_input, token, start - token, last_token);
                }
                
                // Add $ as a token
                add_token_to_list(parsed_input, start, 1, last_token);
                add_special_char(parsed_input, DOLLAR, *position);
                start++;
                (*position)++;
                
                token = start;
                
                // Handle $(cmd) case
                if (*start == '(')
                {
                    // Find the closing parenthesis
                    char *cmd_end = start;
                    while (*cmd_end && *cmd_end != ')')
                        cmd_end++;
                    
                    if (*cmd_end == ')')
                        cmd_end++; // Include the closing parenthesis
                    
                    // Add (cmd) as a single token
                    add_token_to_list(parsed_input, start, cmd_end - start, last_token);
                    start = cmd_end;
                    *position += cmd_end - token;
                }
                else
                {
                    // Handle variable name: take only alphanumeric chars and underscore
                    while (*start && (isalnum(*start) || *start == '_'))
                        start++;
                    
                    // Add variable name as a token if found
                    if (start > token)
                    {
                        add_token_to_list(parsed_input, token, start - token, last_token);
                        *position += start - token;
                    }
                }
                token = start;
            }
            else
            {
                start++;
                (*position)++;
            }
        }
        
        // Add any remaining text before closing quote
        if (start > token)
        {
            add_token_to_list(parsed_input, token, start - token, last_token);
        }
        
        if (*start == quote_char)
        {
            // Add closing quote as a token
            add_token_to_list(parsed_input, start, 1, last_token);
            add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
            start++;
            (*position)++;
        }
        *input = start;
        return NULL;
    }
    else
    {
        while (*start && *start != ' ' && *start != '\t' && *start != '\'' && *start != '"')
        {
            if (*start == '$' || *start == '|' || *start == '<' || *start == '>')
            {
                // Add any content before the special char as a token
                if (start > token)
                {
                    add_token_to_list(parsed_input, token, start - token, last_token);
                }

                if ((*start == '<' && *(start + 1) == '<') || 
                    (*start == '>' && *(start + 1) == '>'))
                {
                    // Add << or >> as a single token
                    add_token_to_list(parsed_input, start, 2, last_token);
                    add_special_char(parsed_input, (*start == '<') ? REDIR_IN : REDIR_OUT, *position); //ternaries to be removed
                    add_special_char(parsed_input, (*start == '<') ? REDIR_IN : REDIR_OUT, *position + 1);
                    start += 2;
                    *position += 2;
                }
                else
                {
                    // Add single special char as a token
                    add_token_to_list(parsed_input, start, 1, last_token);
                    add_special_char(parsed_input, (e_special_char)*start, *position);
                    start++;
                    (*position)++;
                }
                token = start;
            }
            else
            {
                start++;
                (*position)++;
            }
        }
        
        // Add any remaining content as a token
        if (start > token)
        {
            add_token_to_list(parsed_input, token, start - token, last_token);
        }
        
        *input = start;
        return NULL;
    }
}

static bool is_builtin(const char *cmd) {
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    
    for (int i = 0; builtins[i]; i++) {
        if (strcmp(cmd, builtins[i]) == 0)
            return true;
    }
    return false;
}

static void assign_token_roles(t_parsed_input *parsed_input) {
    t_token *current = parsed_input->token;
    bool in_quotes = false;
    bool expect_command = true;
    bool is_echo = false;
    char quote_type = 0;
    
    while (current) {
        // Handle quotes
        if (current->start[0] == '\'' || current->start[0] == '"') {
            if (!in_quotes) {
                in_quotes = true;
                quote_type = current->start[0];
            } else if (current->start[0] == quote_type) {
                in_quotes = false;
                quote_type = 0;
            }
            current->role = ROLE_DELIMITER;
            current = current->next;
            continue;
        }
        
        // If we're in single quotes, everything is a string
        if (in_quotes && quote_type == '\'') {
            current->role = ROLE_STRING;
            current = current->next;
            continue;
        }
        
        // Special handling for double quotes - only expand variables
        if (in_quotes && quote_type == '"') {
            if (current->start[0] == '$') {
                current->role = ROLE_VARIABLE;
            } else {
                current->role = ROLE_STRING;
            }
            current = current->next;
            continue;
        }
        
        // Handle special characters
        if (strchr("|<>", current->start[0])) {
            current->role = ROLE_DELIMITER;
            expect_command = true;
            current = current->next;
            continue;
        }
        
        // Handle commands and their arguments
        if (expect_command) {
            if (is_builtin(current->start)) {
                current->role = ROLE_BUILTIN;
                is_echo = (strcmp(current->start, "echo") == 0);
            } else {
                // Here you would check if it's an executable in PATH
                current->role = ROLE_EXECUTABLE;
            }
            expect_command = false;
        } else {
            // Handle echo -n option specifically
            if (is_echo && strcmp(current->start, "-n") == 0) {
                current->role = ROLE_OPTION;
            }
            // Handle variables outside quotes
            else if (current->start[0] == '$') {
                current->role = ROLE_VARIABLE;
            }
            // Everything else is an argument
            else {
                current->role = ROLE_ARGUMENT;
            }
        }
        
        current = current->next;
    }
}

static int tokenize_input(t_parsed_input *parsed_input, char *input)
{
    int position = 0;
    t_token *last_token = NULL;

    while (*input)
    {
        get_next_token(&input, parsed_input, &position, &last_token);
    }

    // Count total tokens
    t_token *current = parsed_input->token;
    parsed_input->token_count = 0;
    while (current)
    {
        parsed_input->token_count++;
        current = current->next;
    }
    
    return 1;
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;
    int i;

    init_parsed_input(&parsed_input);
    parsed_input.special_char = malloc(sizeof(t_special_char_struct) * 1024);
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
    assign_token_roles(&parsed_input);
    i = 0;
    for (t_token *current = parsed_input.token; current != NULL; current = current->next)
    {
        printf("Token %d: '%s' (Role: %d)\n", i++, current->start, current->role);
    }
    
    printf("\nSpecial characters:\n");
    for (i = 0; i < parsed_input.special_char_count; i++)
    {
        printf("special char: %c position: %d\n",
               parsed_input.special_char[i].type,
               parsed_input.special_char[i].position);
    }
    
    free_parsed_input(&parsed_input);
    return 0;
}
