/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokenizer2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:38:48 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/12 12:24:57 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int assign_roles(t_parsed_input *parsed_input)
{
    t_token *current;
    int     expect_command;

    current = parsed_input->token;
    expect_command = 1;
    while (current)
    {
        if (current->role == ROLE_DEFAULT)
        {
            if (expect_command && !is_builtin_command)
            {
                current->role = ROLE_EXECUTABLE;
                expect_command = 0;
            }
            else if (is_operator(current->start))
            {
                current->role = ROLE_PIPE;
                expect_command = 1;
            }
            else
                current->role = ROLE_ARGUMENT;
        }
        current = current->next;
    }
    return 1;
}

int is_operator(char *str)
{
    return (ft_strcmp(str, "|") == 0 || ft_strcmp(str, "<") == 0 ||
            ft_strcmp(str, ">") == 0);
}

void print_tokens(t_parsed_input *parsed_input)
{
    t_token *current = parsed_input->token;
    int i = 0;
    while (current)
    {
        printf("Token %d: '%s', Role: %d, Quote State: %d\n",
               i++, current->start, current->role, current->quote_state);
        current = current->next;
    }
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;

    init_parsed_input(&parsed_input);
    if (!initial_tokenization(&parsed_input, input))
        return (free_tokens(parsed_input.token), 1);
    if (!refine_tokens(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    if (!expand_variables(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    if (!assign_roles(&parsed_input))
        return (free_tokens(parsed_input.token), 1);
    print_tokens(&parsed_input); // For debugging
    // Execute commands
    free_tokens(parsed_input.token);
    return 0;
}