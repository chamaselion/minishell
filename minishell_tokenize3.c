/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:21:04 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/12 11:25:35 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin_command(const char *cmd)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    int i = 0;

    while (builtins[i])
    {
        if (ft_strcmp(cmd, builtins[i]) == 0) // ft_strcmp in utils
            return 1;
        i++;
    }
    return 0;
}

void assign_role(t_token *token, int *expect_command)
{
    if (*expect_command)
    {
        if (is_builtin_command(token->start))
            token->role = ROLE_BUILTIN;
        else
            token->role = ROLE_EXECUTABLE;
        *expect_command = 0;
    }
    else if (ft_strcmp(token->start, "|") == 0)
    {
        token->role = ROLE_PIPE;
        *expect_command = 1;
    }
    else if (ft_strcmp(token->start, "<") == 0 || ft_strcmp(token->start, ">") == 0 ||
             ft_strcmp(token->start, "<<") == 0 || ft_strcmp(token->start, ">>") == 0)
    {
        token->role = ROLE_REDIRECT;
    }
    else
    {
        token->role = ROLE_ARGUMENT;
    }
}

int assign_roles(t_parsed_input *parsed_input)
{
    t_token *current;
    int expect_command;

    current = parsed_input->token;
    expect_command = 1;
    while (current)
    {
        if (current->role == ROLE_DEFAULT)
            assign_role_based_on_position(current, &expect_command);
        current = current->next;
    }
    return 1;
}

void assign_role_based_on_position(t_token *token, int *expect_command)
{
    if (*expect_command)
    {
        if (is_builtin_command(token->start))
            token->role = ROLE_BUILTIN;
        else
            token->role = ROLE_EXECUTABLE;
        *expect_command = 0;
    }
    else
        token->role = ROLE_ARGUMENT;

    if (ft_strcmp(token->start, "|") == 0)
    {
        token->role = ROLE_PIPE;
        *expect_command = 1;
    }
    else if (is_redirection(token->start))
    {
        token->role = ROLE_REDIRECT;
        *expect_command = 1;
    }
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

/*void insert_token(t_parsed_input *parsed_input, t_token *new_token, t_token *prev)
{
    if (prev)
    {
        new_token->next = prev->next;
        if (prev->next)
            prev->next->prev = new_token;
        prev->next = new_token;
        new_token->prev = prev;
    }
    else
    {
        new_token->next = parsed_input->token;
        if (parsed_input->token)
            parsed_input->token->prev = new_token;
        parsed_input->token = new_token;
    }
    if (!new_token->next)
        parsed_input->last_token = new_token;
    parsed_input->token_count++;
}*/
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
        if (i > start)
        {
            if (!add_token(parsed_input, &input[start], i - start, quote_state))
                return 0;
        }
    }
    return 1;
}
