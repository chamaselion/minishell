/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:17:29 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/02 16:24:34 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_input(char *input)
{
    t_parsed_input parsed_input;
    int status;

    init_parsed_input(&parsed_input);
    parsed_input.special_char = malloc(sizeof(t_special_char_struct) * 1024);
    if (!parsed_input.special_char)
        return (printf("Error: Memory allocation failed\n"), 1);
    status = tokenize_input(&parsed_input, input);
    if (!status)
    {
        free_parsed_input(&parsed_input);
        return (1);
    }
    assign_token_roles(&parsed_input);
    print_tokens(&parsed_input);
    print_special_chars(&parsed_input);
    free_parsed_input(&parsed_input);
    return (0);
}