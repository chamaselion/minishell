/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:26:43 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/02 16:27:44 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void assign_token_roles(t_parsed_input *parsed_input)
{
    t_token *current;
    int in_quotes;
    int expect_command;
    int is_echo;
    char quote_type;

    init_role_vars(&in_quotes, &expect_command, &is_echo, &quote_type);
    current = parsed_input->token;
    while (current)
    {
        if (is_quote_char(current->start[0]))
            handle_quote_role(&current, &in_quotes, &quote_type);
        else if (in_quotes)
            handle_quoted_content(current, quote_type);
        else if (is_special_char(current->start[0]))
            handle_special_char_role(&current, &expect_command);
        else
            handle_command_or_arg(current, &expect_command, &is_echo);
        current = current->next;
    }
}

static void handle_quote_role(t_token **current, int *in_quotes, char *quote_type)
{
    if (!*in_quotes)
    {
        *in_quotes = 1;
        *quote_type = (*current)->start[0];
    }
    else if ((*current)->start[0] == *quote_type)
    {
        *in_quotes = 0;
        *quote_type = 0;
    }
    (*current)->role = ROLE_DELIMITER;
}

static void handle_quoted_content(t_token *current, char quote_type)
{
    if (quote_type == '\'')
        current->role = ROLE_STRING;
    else if (current->start[0] == '$')
        current->role = ROLE_VARIABLE;
    else
        current->role = ROLE_STRING;
}