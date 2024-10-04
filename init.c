/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2024/10/04 14:20:45 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parsed_input(t_parsed_input *parsed_input)
{
	static e_special_char delimiters[] = {T_SPACE, T_TAB, T_NEWLINE, END_OF_FILE};
	
	parsed_input->token = NULL;
	parsed_input->delimiters = (char*)delimiters;
	parsed_input->commands = NULL;
	parsed_input->token_count = 0;
}

void	init_special_char_handling(t_special_char_struct *special_char)
{
	special_char->special_char = NULL;
	special_char->function = NULL;
	special_char->status = -1;
}

void	init_command(char *command_token)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (command == NULL)
		return ;
	command->command = ft_strdup(command_token);
	command->input = NULL;
	command->args = NULL;
	command->output = NULL;
}

void init_shell(t_shell *shell) 
{
    shell->commands = NULL;
    getcwd(shell->cwd, sizeof(shell->cwd));
}
