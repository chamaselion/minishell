/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:19:01 by mnaumann          #+#    #+#             */
/*   Updated: 2024/10/30 14:03:55 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool is_builtin(const char *cmd) {
    static const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    
    for (int i = 0; builtins[i]; i++) {
        if (strcmp(cmd, builtins[i]) == 0)
            return true;
    }
    return false;
}

static int process_quoted_token(t_token *current, bool *in_quotes, char *quote_type) {
    if (!*in_quotes) {
        *in_quotes = true;
        *quote_type = current->start[0];
    } else if (current->start[0] == *quote_type) {
        *in_quotes = false;
        *quote_type = 0;
    }
    current->role = ROLE_DELIMITER;
    return current->role;
}

static int process_quoted_content(t_token *current, char quote_type) {
    if (quote_type == '\'')
        current->role = ROLE_STRING;
    else if (current->start[0] == '$')
        current->role = ROLE_VARIABLE;
    else
        current->role = ROLE_STRING;
    return current->role;
}

static int process_delimiter_token(t_token *current, bool *expect_cmd) {
    current->role = ROLE_DELIMITER;
    *expect_cmd = true;
    return current->role;
}

static int process_command_token(t_token *current, bool *expect_cmd, bool *is_echo) {
    if (is_builtin(current->start)) {
        current->role = ROLE_BUILTIN;
        *is_echo = (strcmp(current->start, "echo") == 0);
    } else {
        current->role = ROLE_EXECUTABLE;
    }
    *expect_cmd = false;
    return current->role;
}

static int process_argument_token(t_token *current, bool is_echo) {
    if (is_echo && strcmp(current->start, "-n") == 0)
        current->role = ROLE_OPTION;
    else if (current->start[0] == '$')
        current->role = ROLE_VARIABLE;
    else
        current->role = ROLE_ARGUMENT;
    return current->role;
}

static void assign_token_roles(t_parsed_input *parsed_input) {
    t_token *current = parsed_input->token;
    bool in_quotes = false;
    bool expect_command = true;
    bool is_echo = false;
    char quote_type = 0;
    
    while (current) {
        if (current->start[0] == '\'' || current->start[0] == '"')
            process_quoted_token(current, &in_quotes, &quote_type);
        else if (in_quotes && quote_type == '\'')
            process_quoted_content(current, quote_type);
        else if (in_quotes && quote_type == '"')
            process_quoted_content(current, quote_type);
        else if (strchr("|<>", current->start[0]))
            process_delimiter_token(current, &expect_command);
        else if (expect_command)
            process_command_token(current, &expect_command, &is_echo);
        else
            process_argument_token(current, is_echo);
        
        current = current->next;
    }
}

static void add_special_char(t_parsed_input *parsed_input, e_special_char type, int position) {
    parsed_input->special_char[parsed_input->special_char_count].type = type;
    parsed_input->special_char[parsed_input->special_char_count].position = position;
    parsed_input->special_char_count++;
}

static void add_token_to_list(t_parsed_input *parsed_input, char *token_start, int length, t_token **last_token) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        printf("Error: Failed to allocate memory for token\n");
        return;
    }
    init_token(new_token);

    new_token->start = malloc(length + 1);
    if (!new_token->start) {
        printf("Error: Failed to allocate memory for token content\n");
        free(new_token);
        return;
    }
    strncpy(new_token->start, token_start, length);
    new_token->start[length] = '\0';
    new_token->length = length;

    if (*last_token) {
        (*last_token)->next = new_token;
        new_token->prev = *last_token;
    } else
        parsed_input->token = new_token;

    *last_token = new_token;
}

static char *get_next_token(char **input, t_parsed_input *parsed_input, int *position, t_token **last_token) {
    char *start = *input;
    char *token = start;
    char quote_char = '\0';

    while (*start == ' ' || *start == '\t') {
        start++;
        (*position)++;
    }
    token = start;

    if (*start == '\'' || *start == '"') {
        quote_char = *start;
        add_token_to_list(parsed_input, start, 1, last_token);
        add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
        
        start++;
        (*position)++;
        token = start;

        while (*start && *start != quote_char) {
            if (*start == '$' && quote_char == '"') {
                if (start > token)
                    add_token_to_list(parsed_input, token, start - token, last_token);
                
                add_token_to_list(parsed_input, start, 1, last_token);
                add_special_char(parsed_input, DOLLAR, *position);
                start++;
                (*position)++;
                
                token = start;
                
                if (*start == '(') {
                    char *cmd_end = start;
                    while (*cmd_end && *cmd_end != ')')
                        cmd_end++;
                    
                    if (*cmd_end == ')')
                        cmd_end++;
                    
                    add_token_to_list(parsed_input, start, cmd_end - start, last_token);
                    start = cmd_end;
                    *position += cmd_end - token;
                } else {
                    while (*start && (isalnum(*start) || *start == '_'))
                        start++;
                    
                    if (start > token)
                    {
                        add_token_to_list(parsed_input, token, start - token, last_token);
                        *position += start - token;
                    }
                }
                token = start;
            } else {
                start++;
                (*position)++;
            }
        }
        
        if (start > token)
            add_token_to_list(parsed_input, token, start - token, last_token);
        
        if (*start == quote_char) {
            add_token_to_list(parsed_input, start, 1, last_token);
            add_special_char(parsed_input, (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE, *position);
            start++;
            (*position)++;
        }
        *input = start;
        return NULL;
    } else {
        while (*start && *start != ' ' && *start != '\t' && *start != '\'' && *start != '"') {
            if (*start == '$' || *start == '|' || *start == '<' || *start == '>') {
                if (start > token)
                    add_token_to_list(parsed_input, token, start - token, last_token);

                if ((*start == '<' && *(start + 1) == '<') || (*start == '>' && *(start + 1) == '>')) {
                    add_token_to_list(parsed_input, start, 2, last_token);
                    add_special_char(parsed_input, (*start == '<') ? REDIR_IN : REDIR_OUT, *position);
                    add_special_char(parsed_input, (*start == '<') ? REDIR_IN : REDIR_OUT, *position + 1);
                    start += 2;
                    *position += 2;
                } else {
                    add_token_to_list(parsed_input, start, 1, last_token);
                    add_special_char(parsed_input, (e_special_char)*start, *position);
                    start++;
                    (*position)++;
                }
                token = start;
            } else {
                start++;
                (*position)++;
            }
        }
        
        if (start > token)
            add_token_to_list(parsed_input, token, start - token, last_token);
        
        *input = start;
        return NULL;
    }
}

int tokenize_input(t_parsed_input *parsed_input, char *input) {
    int position = 0;
    t_token *last_token = NULL;

    while (*input)
        get_next_token(&input, parsed_input, &position, &last_token);

    t_token *current = parsed_input->token;
    parsed_input->token_count = 0;
    while (current) {
        parsed_input->token_count++;
        current = current->next;
    }
    
    return 1;
}
