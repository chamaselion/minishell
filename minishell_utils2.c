/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:52:31 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/16 11:51:01 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int get_quote_state(char c)
{
if (c == '\'')
        return WITHIN_SINGLE_QUOTE;
    else
        return WITHIN_DOUBLE_QUOTE;
}

int is_matching_quote(char c, int state)
{
    return ((c == '\'' && state == WITHIN_SINGLE_QUOTE) ||
            (c == '\"' && state == WITHIN_DOUBLE_QUOTE));
}


int is_quote_char(char c)
{
    return c == '\'' || c == '"';
}

int get_quote_type(char c)
{
    if (c == '\'')
        return WITHIN_SINGLE_QUOTE;
    if (c == '"')
        return WITHIN_DOUBLE_QUOTE;
    return NO_QUOTE;
}

int is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

int is_builtin_command(const char *cmd)
{
    const char *builtins[] = {"echo", "cd", "pwd", "unset", "export", "env", "exit"};
    int i = 0;
    while (i < 7)
    {
        if (ft_strcmp(cmd, builtins[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}
