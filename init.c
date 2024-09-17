/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2024/09/17 18:10:39 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parsed_input(t_parsed_input parsed_input)
{
	parsed_input.token = NULL;
	parsed->delimiters = e_special_char[SPACE, TAB, NEWLINE, CARRIAGE_RETURN, END_OF_FILE];
	parsed->commands = NULL;
	parsed->token_count = 0;
}

void	init_special_char_handling(t_special_char_struct *special_char)
{
	special_char->char = NULL;
	special_char->function = NULL;
	special_char->status = -1;
}

void	init_command(char *command_token)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (command == NULL)
		return ;
	command->command = command_token;
	command->input = NULL;
	command->args = NULL;
	command->output = NULL;
	command->pipe = NULL;
}
