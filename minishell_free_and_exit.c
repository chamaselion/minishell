
/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free_and_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:09:32 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/10 13:09:36 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void free_command(t_command *cmd) 
{
    int i = 0;

    free(cmd->command);
    while (cmd->args && cmd->args[i]) 
    {
        free(cmd->args[i]);
        i++;
    }
    free(cmd->input);
    free(cmd->output);
    free(cmd);
}

void free_commands(t_command *cmd) 
{
    while (cmd) 
    {
        t_command *next = cmd->next;
        free_command(cmd);
        cmd = next;
    }
}

int free_and_return(char *content, int ret)
{
    free(content);
    return ret;
}

int free_and_fail(char *content, char *message)
{
    free(content);
    ft_putstr_fd(message, STDERR_FILENO);
    return 0;
}

void free_tokens(t_token *token)
{
    t_token *current;
    t_token *next;

    current = token;
    while (current)
    {
        next = current->next;
        free(current);
        current = next;
    }
}