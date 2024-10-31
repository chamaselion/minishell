/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:03:44 by mnaumann          #+#    #+#             */
/*   Updated: 2024/10/30 10:57:15 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int process_command_token(t_token *current, bool *expect_cmd, bool *is_echo)
{
    if (is_builtin(current->start))
    {
        current->role = ROLE_BUILTIN;
        *is_echo = (strcmp(current->start, "echo") == 0);
    }
    else
        current->role = ROLE_EXECUTABLE;
    *expect_cmd = false;
    return (current->role);
}

int process_argument_token(t_token *current, bool is_echo)
{
    if (is_echo && strcmp(current->start, "-n") == 0)
        current->role = ROLE_OPTION;
    else if (current->start[0] == '$')
        current->role = ROLE_VARIABLE;
    else
        current->role = ROLE_ARGUMENT;
    return (current->role);
}

void print_parsing_results(t_parsed_input *parsed_input)
{
    t_token *current;
    int     i;

    i = 0;
    current = parsed_input->token;
    while (current)
    {
        printf("Token %d: '%s' (Role: %d)\n", i++, current->start, 
            current->role);
        current = current->next;
    }
    i = 0;
    while (i < parsed_input->special_char_count)
    {
        printf("special char: %c position: %d\n",
            parsed_input->special_char[i].type,
            parsed_input->special_char[i].position);
        i++;
    }
}
