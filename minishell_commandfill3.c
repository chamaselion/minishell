/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commandfill3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:49 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 14:03:11 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_last_empty_command(t_command *head_cmd, t_command *current_cmd)
{
	t_command	*prev_cmd;

	if (current_cmd->command == NULL && current_cmd->args == NULL)
	{
		prev_cmd = head_cmd;
		while (prev_cmd->next != current_cmd)
		{
			prev_cmd = prev_cmd->next;
		}
		prev_cmd->next = NULL;
		free(current_cmd);
	}
}

int	update_redirection_last_path(t_token *ct)
{
	if (strcmp(ct->content, ">") == 0)
		return (1);
	else if (strcmp(ct->content, "<") == 0)
		return (2);
	else if (strcmp(ct->content, ">>") == 0)
		return (3);
	else if (strcmp(ct->content, "<<") == 0)
		return (4);
	return (0);
}

int	handle_redirect_link(t_token *ct, t_command *current_cmd)
{
	if (strcmp(ct->content, ">") == 0)
		add_redirect(&current_cmd->output_redirections, ct->next);
	else if (strcmp(ct->content, "<") == 0)
		add_redirect(&current_cmd->input_redirections, ct->next);
	else if (strcmp(ct->content, ">>") == 0)
		add_redirect(&current_cmd->append_redirections, ct->next);
	else if (strcmp(ct->content, "<<") == 0)
		add_redirect(&current_cmd->heredoc_redirections, ct->next);
	if (current_cmd->redir_count < 64)
        current_cmd->redir_order[current_cmd->redir_count++] = update_redirection_last_path(ct);
	return (0);
}

void	handle_pipe_link(t_command *current_cmd)
{
	current_cmd->relation_type = 6;
	current_cmd->related_to = current_cmd->next;
}

int	handle_redirect_and_update(t_token **ct, t_command *current_cmd,
		t_command **cmd)
{
	if (handle_redirect_link(*ct, current_cmd) == 0)
	{
		if ((*ct)->next)
			*ct = (*ct)->next;
		current_cmd->relation_type = 5;
		return (0);
	}
	else
	{
		free_commands(*cmd);
		return (1);
	}
}

int	link_commands_and_tokens(t_token *tokens, t_command *cmd)
{
	t_token		*ct;
	t_command	*current_cmd;

	if (validate_token_syntax(tokens) == SYNTAX_ERROR)
	{
		ft_putstr_fd("Error: syntax error\n", STDERR_FILENO);
		update_exit_code(cmd->shell, 2);
		return (1);
	}
	ct = tokens;
	current_cmd = cmd;
	while (ct)
	{
		if (ct->role == 5)
		{
			if (handle_redirect_and_update(&ct, current_cmd, &cmd) == 1)
				return (1);
		}
		else if (ct->role == 6)
		{
			if (current_cmd->next)
			{
				handle_pipe_link(current_cmd);
				current_cmd = current_cmd->next;
			}
		}
		ct = ct->next;
	}
	ft_function_marker(cmd);
	return (0);
}
