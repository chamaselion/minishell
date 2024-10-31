/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:27:50 by mnaumann          #+#    #+#             */
/*   Updated: 2024/10/30 15:01:55 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int process_quoted_token(t_token *current, bool *in_quotes, char *quote_type)
{
    if (!*in_quotes)
    {
        *in_quotes = true;
        *quote_type = current->start[0];
    }
    else if (current->start[0] == *quote_type)
    {
        *in_quotes = false;
        *quote_type = 0;
    }
    current->role = ROLE_DELIMITER;
    return (current->role);
}

int process_quoted_content(t_token *current, char quote_type)
{
    if (quote_type == '\'')
        current->role = ROLE_DELIMITER;
    else if (current->start[0] == '$')
        current->role = ROLE_ASSIGNMENTOPERATOR;
    else
        current->role = ROLE_DEFAULT;
    return (current->role);
}

int process_delimiter_token(t_token *current, bool *expect_cmd)
{
    current->role = ROLE_DELIMITER;
    *expect_cmd = true;
    return (current->role);
}