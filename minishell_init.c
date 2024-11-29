/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2024/11/29 15:45:23 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_special_char_handling(t_special_char_struct *special_char)
{
    special_char->type = END_OF_FILE;
    special_char->count = 0;
    special_char->position = -1;
}

void init_command(t_command *command)
{
    command->command = NULL;
    command->input = NULL;
    command->args = malloc(sizeof(char *) * MAX_ARGS);
    command->arg_count = 0;
    command->output = NULL;
    command->next = NULL;
}

void init_shell(t_shell *shell) 
{
    shell->commands = NULL;
    getcwd(shell->cwd, sizeof(shell->cwd));
}

void init_token(t_token *token, t_raw_token *t_raw_token)
{
    token->next = NULL;
    token->prev = NULL;
    token->role = ROLE_DEFAULT;
    token->quote_state = t_raw_token->quote_state;
}
