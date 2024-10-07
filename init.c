/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2024/10/07 15:34:55 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_parsed_input(t_parsed_input *parsed_input)
{
    static char delimiters[] = {T_SPACE, T_TAB, T_NEWLINE, END_OF_FILE};
    
    parsed_input->token = NULL;
    parsed_input->delimiters = delimiters;
    parsed_input->commands = NULL;
    parsed_input->token_count = 0;
}

void init_special_char_handling(t_special_char_struct *special_char)
{
    special_char->special_char = NULL;
    special_char->function = NULL;
    special_char->status = -1;
}

void init_command(t_command *command)
{
    command->command = NULL;
    command->input = NULL;
    command->args = malloc(sizeof(char *) * MAX_ARGS);
    command->arg_count = 0;
    command->output = NULL;
    command->next = NULL;
    command->append = 0;
}

void init_shell(t_shell *shell) 
{
    shell->commands = NULL;
    getcwd(shell->cwd, sizeof(shell->cwd));
}
