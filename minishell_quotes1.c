/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_quotes1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:15:08 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/06 15:32:33 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_quote_char(char c)
{
    return (c == '\'' || c == '\"');
}

int toggle_quote_state(char c, int quote_state)
{
    if (c == '\'' && quote_state == NO_QUOTE)
        return WITHIN_SINGLE_QUOTE;
    if (c == '\'' && quote_state == WITHIN_SINGLE_QUOTE)
        return NO_QUOTE;
    if (c == '\"' && quote_state == NO_QUOTE)
        return WITHIN_DOUBLE_QUOTE;
    if (c == '\"' && quote_state == WITHIN_DOUBLE_QUOTE)
        return NO_QUOTE;
    return quote_state;
}

char *process_token_content(const char *str, int *quote_state)
{
    char *result;
    int i;
    int j;

    result = malloc(ft_strlen(str) + 1);
    if (!result)
        return NULL;
    i = -1;
    j = 0;
    while (str[++i])
    {
        if (str[i] == '\'' && *quote_state == NO_QUOTE)
            *quote_state = WITHIN_SINGLE_QUOTE;
        else if (str[i] == '\'' && *quote_state == WITHIN_SINGLE_QUOTE)
            *quote_state = NO_QUOTE;
        else if (str[i] == '\"' && *quote_state == NO_QUOTE)
            *quote_state = WITHIN_DOUBLE_QUOTE;
        else if (str[i] == '\"' && *quote_state == WITHIN_DOUBLE_QUOTE)
            *quote_state = NO_QUOTE;
        else
            result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}

static int handle_unclosed_quote(int *quote_state)
{
    if (*quote_state == WITHIN_SINGLE_QUOTE)
        *quote_state = UNCLOSED_SINGLE_QUOTE;
    else if (*quote_state == WITHIN_DOUBLE_QUOTE)
        *quote_state = UNCLOSED_DOUBLE_QUOTE;
    else
        return 1; // No unclosed quote
    return 1;
}

static char *get_prompt(int quote_state)
{
    if (quote_state == UNCLOSED_SINGLE_QUOTE)
        return "quote> ";
    else
        return "dquote> ";
}

static int read_line_and_update(char **new_input, char *prompt)
{
    char *line;

    line = readline(prompt);
    if (!line)
    {
        ft_putstr_fd("Error: Unexpected EOF while looking for matching quote\n", STDERR_FILENO);
        return 0;
    }
    *new_input = ft_strjoin_and_free(*new_input, "\n");
    *new_input = ft_strjoin_and_free(*new_input, line);
    free(line);
    if (!*new_input)
    {
        ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
        return 0;
    }
    return 1;
}

static int reprocess_token(t_token *token, char **new_input, int *quote_state)
{
    char *new_content;

    free(token->start);
    token->start = *new_input;
    token->length = ft_strlen(token->start);
    if (*quote_state == UNCLOSED_SINGLE_QUOTE) 
     *quote_state = WITHIN_SINGLE_QUOTE;
    else 
     *quote_state = WITHIN_DOUBLE_QUOTE;
    new_content = process_token_content(token->start, quote_state);
    if (!new_content)
    {
        ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
        return 0;
    }
    free(token->start);
    token->start = new_content;
    token->length = ft_strlen(new_content);
    handle_unclosed_quote(quote_state);
    token->quote_state = *quote_state;
    return 1;
}

static int prompt_for_unclosed_quote(t_token *token)
{
    char    *new_input;
    char    *prompt;
    int     quote_state;

    prompt = get_prompt(token->quote_state);
    new_input = ft_strdup(token->start);
    if (!new_input)
        return 0;
    quote_state = token->quote_state;
    while (quote_state == UNCLOSED_SINGLE_QUOTE || quote_state == UNCLOSED_DOUBLE_QUOTE)
    {
        if (!read_line_and_update(&new_input, prompt))
        {
            free(new_input);
            return 0;
        }
        if (!reprocess_token(token, &new_input, &quote_state))
        {
            free(new_input);
            return 0;
        }
        prompt = get_prompt(quote_state);
    }
    return 1;
}

int check_unclosed_quotes(t_parsed_input *parsed_input)
{
    t_token *current;

    current = parsed_input->token;
    while (current)
    {
        if (current->quote_state == UNCLOSED_SINGLE_QUOTE ||
            current->quote_state == UNCLOSED_DOUBLE_QUOTE)
        {
            if (!prompt_for_unclosed_quote(current))
                return 0;
        }
        current = current->next;
    }
    return 1;
}

int process_quotes(t_parsed_input *parsed_input)
{
    t_token *current;
    char *new_content;
    int quote_state;

    current = parsed_input->token;
    while (current)
    {
        quote_state = NO_QUOTE;
        new_content = process_token_content(current->start, &quote_state);
        if (!new_content)
            return (ft_putstr_fd("Error: Memory allocation failed\n", 2), 0);
        free(current->start);
        current->start = new_content;
        current->length = ft_strlen(new_content);
        handle_unclosed_quote(&quote_state);
        current->quote_state = quote_state;
        current = current->next;
    }
    return 1;
}
