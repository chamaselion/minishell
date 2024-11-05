/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:03:44 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/02 16:29:17 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_command_or_arg(t_token *current, int *expect_command,
    int *is_echo)
{
    if (*expect_command)
        handle_command(current, expect_command, is_echo);
    else
        handle_argument(current, *is_echo);
}

static void handle_command(t_token *current, int *expect_command, int *is_echo)
{
    if (is_builtin(current->start))
    {
        current->role = ROLE_BUILTIN;
        *is_echo = (ft_strcmp(current->start, "echo") == 0);
    }
    else if (search_command(current->start))
        current->role = ROLE_EXECUTABLE;
    *expect_command = 0;
}

static void handle_argument(t_token *current, int is_echo)
{
    if (is_echo && ft_strcmp(current->start, "-n") == 0)
        current->role = ROLE_OPTION;
    else if (current->start[0] == '$')
        current->role = ROLE_VARIABLE;
    else
        current->role = ROLE_ARGUMENT;
}
