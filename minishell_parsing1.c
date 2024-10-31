/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:17:29 by mnaumann          #+#    #+#             */
/*   Updated: 2024/10/30 14:38:16 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool is_builtin(const char *cmd)
{
    !!! changed to enumconst char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    }; //would like to change to an enum as well to be consistent
    int i;
    
    i = 0;
    while (builtins[i])
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return (true);
        i++;
    }
    return (false);
}

void assign_token_roles(t_parsed_input *parsed_input)
{
    t_token *current;
    bool    in_quotes;
    bool    expect_cmd;
    bool    is_echo;
    char    quote_type;

    current = parsed_input->token;
    in_quotes = false;
    expect_cmd = true;
    is_echo = false;
    quote_type = 0;
    while (current)
    {
        if (process_token(current, &in_quotes, &expect_cmd, &is_echo, 
            &quote_type) == ROLE_ERROR)
            break ;
        current = current->next;
    }
}

int process_token(t_token *current, bool *in_quotes, bool *expect_cmd,
    bool *is_echo, char *quote_type)
{
    if (current->start[0] == '\'' || current->start[0] == '"')
        return (process_quoted_token(current, in_quotes, quote_type));
    if (*in_quotes)
        return (process_quoted_content(current, *quote_type));
    if (strchr("|<>", current->start[0]))
        return (process_delimiter_token(current, expect_cmd));
    if (*expect_cmd)
        return (process_command_token(current, expect_cmd, is_echo));
    return (process_argument_token(current, *is_echo));
}