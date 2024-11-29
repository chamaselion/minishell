/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:12:59 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/29 16:07:58 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int categorize_token(t_token *token, int command_expected) 
{
    if (is_pipe(token->content))
        return ROLE_PIPE;
    if (is_redirection(token->content))
        return ROLE_REDIRECT;
    if (identify_env_var(token->content))
        return ROLE_VARIABLE;
    if (command_expected) 
    {
        if (is_builtin_command(token->content))
            return ROLE_BUILTIN;
        
        return ROLE_EXECUTABLE;
    }
    return ROLE_ARGUMENT;
}

