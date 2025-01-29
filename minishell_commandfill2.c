/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commandfill2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:30:29 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 13:18:45 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_tokens(t_token *ct, t_command *current_cmd)
{
	while (ct && (ct->role == 3 || ct->role == 0))
	{
		if (ct->role == 3)
		{
			if (allocate_args(current_cmd, ct) == -1)
				return (-1);
		}
		ct = ct->next;
	}
	current_cmd->args = ft_realloc(current_cmd->args, sizeof(char *)
			* current_cmd->arg_count, sizeof(char *) * (current_cmd->arg_count
				+ 1));
	if (current_cmd->args == NULL)
		return (-1);
	current_cmd->args[current_cmd->arg_count] = NULL;
	// current_cmd->args = purge_quotes_from_args(current_cmd);
	return (0);
}

static int	validate_tokens(t_token *tokens, t_command **cmd)
{
	if (!tokens)
	{
		*cmd = NULL;
		return (-1);
	}
	return (0);
}

static int	process_token_loop(t_token *ct, t_command *current_cmd)
{
	while (ct)
	{
		if (ct->role == 1 || ct->role == 2)
		{
			current_cmd->command = ft_strdup(ct->content);
			if (process_tokens(ct->next, current_cmd) == -1)
				return (-1);
			if (current_cmd->next)
				current_cmd = current_cmd->next;
		}
		ct = ct->next;
	}
	return (0);
}

static int	setup_command_list(t_token *tokens, t_command **cmd)
{
	t_command	*current_cmd;

	current_cmd = create_command_list(tokens);
	if (!current_cmd)
		return (-1);
	*cmd = current_cmd;
	return (0);
}

int	fill_command_from_tokens(t_token *tokens, t_command **cmd)
{
	t_command	*current_cmd;

	if (validate_tokens(tokens, cmd) == -1)
		return (-1);
	if (setup_command_list(tokens, cmd) == -1)
		return (-1);
	current_cmd = *cmd;
	if (process_token_loop(tokens, current_cmd) == -1)
		return (-1);
	return (0);
}
