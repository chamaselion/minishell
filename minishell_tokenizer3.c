/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:55:23 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/23 14:06:28 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_raw_token	*create_raw_token(const char *segment, t_quote_state quote_state,
		int position)
{
	t_raw_token	*token;

	token = malloc(sizeof(t_raw_token));
	if (token == NULL)
		return (NULL);
	token->segment = ft_strdup(segment);
	token->quote_state = quote_state;
	token->position = position;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	append_raw_token(t_raw_token **first, t_raw_token **last,
		t_raw_token *new_token)
{
	if (new_token == NULL)
		return ;
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

t_raw_token	*handle_single_quote_mark(int *pos)
{
	char		quote_str[2];
	t_raw_token	*token;

	quote_str[0] = '\'';
	quote_str[1] = '\0';
	token = create_raw_token(quote_str, NO_QUOTE, *pos);
	return (token);
}

t_raw_token	*handle_double_quote_mark(int *pos)
{
	char		quote_str[2];
	t_raw_token	*token;

	quote_str[0] = '"';
	quote_str[1] = '\0';
	token = create_raw_token(quote_str, NO_QUOTE, *pos);
	return (token);
}

t_raw_token *handle_non_quote_segment(const char **input, int *pos)
{
    const char *start = *input;
    char *segment;
    t_raw_token *token;
    int in_quotes = 0;
    char quote_char = 0;

    while (**input && (!is_whitespace(**input) || in_quotes))
    {
        if (**input == '=' && !in_quotes)
        {
            (*input)++;
            while (**input && !is_whitespace(**input))
            {
                if (**input == '"' || **input == '\'')
                {
                    quote_char = **input;
                    (*input)++;
                    while (**input && **input != quote_char)
                        (*input)++;
                    if (**input == quote_char)
                        (*input)++;
                }
                else
                    (*input)++;
            }
            break;
        }
        else if (**input == '"' || **input == '\'')
            break;
        (*input)++;
    }
    if (*input == start)
        return NULL;
    segment = ft_strndup(start, *input - start);
    token = create_raw_token(segment, NO_QUOTE, *pos);
    free(segment);
    return token;
}

/* t_raw_token *handle_non_quote_segment(const char **input, int *pos)
{
    const char *start = *input;
    char *segment;
    t_raw_token *token;
    int in_quotes = 0;
    char quote_char = 0;
    char var_name[256];
    char *write_ptr;

    segment = malloc(MAX_PATH);
    if (!segment)
        return NULL;
    write_ptr = segment;

    while (**input && (!is_whitespace(**input) || in_quotes))
    {
        if (**input == '=' && !in_quotes)
        {
            (*input)++;
            while (**input && !is_whitespace(**input))
            {
                if (**input == '"' || **input == '\'')
                {
                    quote_char = **input;
                    (*input)++;
                    while (**input && **input != quote_char)
                        (*input)++;
                    if (**input == quote_char)
                        (*input)++;
                }
                else if (**input == '$')
                {
                    (*input)++;
                    *input = extract_variable_name(*input, var_name);
                    append_variable_value(var_name, &write_ptr);
                }
                else
                {
                    *write_ptr++ = **input;
                    (*input)++;
                }
            }
            break;
        }
        else if (**input == '"' || **input == '\'')
            break;
        else if (**input == '$')
        {
            (*input)++;
            *input = extract_variable_name(*input, var_name);
            append_variable_value(var_name, &write_ptr);
        }
        else
        {
            *write_ptr++ = **input;
            (*input)++;
        }
    }
    if (*input == start)
    {
        free(segment);
        return NULL;
    }
    *write_ptr = '\0';
    token = create_raw_token(segment, NO_QUOTE, *pos);
    free(segment);
    return token;
}*/

void	print_raw_tokens(t_raw_token *first_token)
{
	int count = 0;
	while (first_token)
	{
		printf("Token %d: '%s' (Quote State: %d, Position: %d)\n",
				count++,
				first_token->segment,
				first_token->quote_state,
				first_token->position);
		if (first_token->next == NULL)
			break ;
		first_token = first_token->next;
	}
}

void	print_tokens(t_token *first_token)
{
	int count = 0;
	while (first_token != NULL) {
    	printf("Token %d: '%s' (Quote State: %d, Position: %d, Command: %d, Role: %d)\n",
           count++,
           first_token->content,
           first_token->quote_state,
           first_token->position,
           first_token->command_expected,
           first_token->role);
    first_token = first_token->next;
	}
	printf("done printing tokens\n");
}
