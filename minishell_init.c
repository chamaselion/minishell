/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2024/10/28 13:00:00 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void init_parsed_input(t_parsed_input *parsed_input)
{
    static char delimiters[] = {' ', '\t', '\n', END_OF_FILE};
    
    parsed_input->token = NULL;
    parsed_input->delimiters = delimiters;
    parsed_input->commands = NULL;
    parsed_input->token_count = 0;
    parsed_input->special_char = NULL;
    parsed_input->special_char_count = 0;
}

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
    command->related_to = NULL;
	command->relation_type = 0;
    command->next = NULL;
    command->is_internal = 0;
	command->input_redirection = NULL;
	command->output_redirection = NULL;
	command->append_redirection = NULL;
	command->heredoc_redirection = NULL;
}

void init_shell(t_shell *shell) 
{
    shell->commands = NULL;
    getcwd(shell->cwd, sizeof(shell->cwd));
}

void init_token(t_token *token)
{
    token->start = NULL;
    token->length = 0;
    token->next = NULL;
    token->prev = NULL;
}
