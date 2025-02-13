/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:16:37 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 21:25:20 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_tokens_and_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands;

	commands = NULL;
	if (tokens)
	{
		if (fill_command_from_tokens(tokens, &commands) != -1)
		{
			shell_to_command(&commands, shell);
			if (link_commands_and_tokens(tokens, commands) == 0 && commands)
			{
				signal_noninteractive();
				handle_pipes(commands);
				free_commands(commands);
			}
		}
	}
}
