/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:55:15 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/10 13:22:36 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Create and initialize a raw token
t_raw_token *create_raw_token(const char *segment, t_quote_state quote_state, int position) 
{
    t_raw_token *token;

    token = malloc(sizeof(t_raw_token));
    if (token == NULL)
        return NULL;
    token->segment = ft_strdup(segment);
    token->quote_state = quote_state;
    token->position = position;
    token->next = NULL;
    token->prev = NULL;
    return token;
}

// Append a token to the linked list
void append_raw_token(t_raw_token **first, t_raw_token **last, t_raw_token *new_token) 
{
    if (new_token == NULL)
        return;
    if (*first == NULL) 
	{
        *first = new_token;
        *last = new_token;
    } 
	else 
	{
        (*last)->next = new_token;
        new_token->prev = *last;
        *last = new_token;
    }
}

// Check if the given string is a valid environment variable name
int is_valid_env_var_name(const char *str) {
    const char *current;

    if (str == NULL || *str == '\0')
        return 0;

    if (!ft_isalpha(*str) && *str != '_')
        return 0;
    current = str + 1;
    while (*current != '\0') 
	{
        if (!ft_isalnum(*current) && *current != '_')
            return 0;
        current++;
    }
    return 1;
}

/* Expand variables and return the full combined string
static char *expand_double_quote_content(const char *content) 
{
    char buffer[1024];
    char var_name[256];
    char *write_ptr;
    int var_idx;
    const char *current;

    ft_memset(buffer, 0, sizeof(buffer));
    write_ptr = buffer;

    current = content;
    while (*current != '\0') {
        if (*current == '$') {
            current++;
            var_idx = 0;

            while (ft_isalnum(*current) || *current == '_') 
			{
                var_name[var_idx] = *current;
                var_idx++;
                current++;
            }
            var_name[var_idx] = '\0';
            if (is_valid_env_var_name(var_name)) 
			{
                char *env_value = expand_env_variable(var_name);
                if (env_value != NULL) 
				{
                    ft_strcpy(write_ptr, env_value);
                    write_ptr += ft_strlen(env_value);
                }
            } 
			else 
			{
                *write_ptr = '$';
                write_ptr++;
                ft_strcpy(write_ptr, var_name);
                write_ptr += ft_strlen(var_name);
            }
        } 
		else 
		{
            *write_ptr = *current;
            write_ptr++;
            current++;
        }
    }
    return ft_strdup(buffer);
}*/

t_raw_token *handle_single_quote_mark(int *pos) 
{
    char quote_str[2];
    t_raw_token *token;

    quote_str[0] = '\'';
    quote_str[1] = '\0';
    token = create_raw_token(quote_str, NO_QUOTE, *pos);
    return token;
}

t_raw_token *handle_double_quote_mark(int *pos) 
{
    char quote_str[2];
    t_raw_token *token;

    quote_str[0] = '"';
    quote_str[1] = '\0';
    token = create_raw_token(quote_str, NO_QUOTE, *pos);
    return token;
}

t_raw_token *handle_double_quote_segment(const char **input, int *pos) 
{
    const char *start;
    char *content;
    char *expanded_content;
    t_raw_token *token;

    (*input)++;
    start = *input;
    while (**input != '\0' && **input != '"')
        (*input)++;
    content = ft_strndup(start, *input - start);
    expanded_content = expand_double_quote_content(content);
    token = create_raw_token(expanded_content, WITHIN_DOUBLE_QUOTE, *pos);
    free(content);
    free(expanded_content);
    return token;
}

// Handle a non-quoted segment
t_raw_token *handle_non_quote_segment(const char **input, int *pos) {
    const char *start;
    char *segment;
    t_raw_token *token;

    start = *input;
    while (**input != '\0' && !is_whitespace(**input) && !is_quote_char(**input)) 
	{
        (*input)++;
    }
    if (*input == start)
        return NULL;
    segment = ft_strndup(start, *input - start);
    token = create_raw_token(segment, NO_QUOTE, *pos);
    free(segment);
    return token;
}

// Handle content within single quotes
t_raw_token *handle_single_quote_segment(const char **input, int *pos) {
    const char *start;
    char *content;
    t_raw_token *token;

    (*input)++;
    start = *input;

    while (**input != '\0' && **input != '\'') 
        (*input)++;
    content = ft_strndup(start, *input - start);
    token = create_raw_token(content, WITHIN_SINGLE_QUOTE, *pos);
    free(content);
    return token;
}

void single_quote_wrapper(const char **input, int *pos, t_raw_token **first, t_raw_token **last) 
{
	t_raw_token *token;

	token = handle_single_quote_mark(pos);
	append_raw_token(first, last, token);
	token = handle_single_quote_segment(input, pos);
	append_raw_token(first, last, token);
	if (**input == '\'') 
	{
		token = handle_single_quote_mark(pos);
		append_raw_token(first, last, token);
		(*input)++;
	}
}

void double_quote_wrapper(const char **input, int *pos, t_raw_token **first, t_raw_token **last) 
{
	t_raw_token *token;

	token = handle_double_quote_mark(pos);
	append_raw_token(first, last, token);
	token = handle_double_quote_segment(input, pos);
	append_raw_token(first, last, token);
	if (**input == '"') 
	{
		token = handle_double_quote_mark(pos);
		append_raw_token(first, last, token);
		(*input)++;
	}
}

// Main tokenizer logic
t_raw_token *handle_input(char *input) {
    t_raw_token *first;
    t_raw_token *last;
    t_raw_token *token;
    int pos;

    first = NULL;
    last = NULL;
    pos = 0;

    while (*input != '\0') 
	{
        input = skip_whitespace(input);
        pos++;
        if (*input == '\'')
			single_quote_wrapper((const char **)&input, &pos, &first, &last);
		else if (*input == '"')
			double_quote_wrapper((const char **)&input, &pos, &first, &last);
		else
		{ 
            token = handle_non_quote_segment((const char **)&input, &pos);
        	append_raw_token(&first, &last, token);
		}
    }
    return first;
}

// Free raw token list
void free_raw_tokens(t_raw_token *first_token) 
{
    t_raw_token *temp;

    while (first_token != NULL) 
	{
        temp = first_token;
        first_token = first_token->next;
        free(temp->segment);
        free(temp);
    }
}
