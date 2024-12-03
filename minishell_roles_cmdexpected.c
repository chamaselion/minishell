/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_roles_cmdexpected.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:28:30 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/03 19:15:30 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_pipe_token(t_token *token, int *command_expected)
{
    token->role = ROLE_PIPE;
    *command_expected = 1;
}

void handle_redirect_token(t_token *token, int *command_expected)
{
    token->role = ROLE_REDIRECT;
    *command_expected = 0;
}

void handle_command_token(t_token *token, int *command_expected)
{
    if (is_builtin_command(token->content))
        token->role = ROLE_BUILTIN;
    /*else if (is_executable(token->content))
        token->role = ROLE_EXECUTABLE;*/
    *command_expected = 0;
}

void assign_token_role(t_token *token, int *command_expected)
{
    if (is_pipe(token->content))
    {
        handle_pipe_token(token, command_expected);
        return;
    }
    if (is_redirection(token->content))
    {
        handle_redirect_token(token, command_expected);
        return;
    }
    if ((is_builtin_command(token->content) /*|| is_executable(token->content)*/) 
        && *command_expected)
    {
        handle_command_token(token, command_expected);
        return;
    }
    if (token->prev && 
        (token->prev->role == ROLE_BUILTIN || token->prev->role == ROLE_EXECUTABLE))
    {
        token->role = ROLE_ARGUMENT;
    }
    if (identify_env_var(token->content))
    {
        token->role = ROLE_VARIABLE;
    }
}

t_token *process_token_list(t_token *token_list)
{
    t_token *current = token_list;
    int command_expected = 1;

    while (current != NULL)
    {
        assign_token_role(current, &command_expected);
        current->command_expected = command_expected;
        current = current->next;
    }
    return token_list;
}

