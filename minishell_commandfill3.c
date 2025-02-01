/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commandfill3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:49 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 16:31:27 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirect_link(t_token *ct, t_command *c_cmd)
{
	if (ft_strcmp(ct->content, ">") == 0)
		add_redirect(&c_cmd->output_redirections, ct->next);
	else if (ft_strcmp(ct->content, "<") == 0)
		add_redirect(&c_cmd->input_redirections, ct->next);
	else if (ft_strcmp(ct->content, ">>") == 0)
		add_redirect(&c_cmd->append_redirections, ct->next);
	else if (ft_strcmp(ct->content, "<<") == 0)
		add_redirect(&c_cmd->heredoc_redirections, ct->next);
	if (c_cmd->redir_count < 64)
		c_cmd->redir_order[c_cmd->redir_count++] = update_redir_lp(ct);
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

int	process_command_tokens(t_token *ct, t_command *current_cmd, t_command *cmd)
{
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
	return (0);
}

int	link_commands_and_tokens(t_token *tokens, t_command *cmd)
{
	t_token		*ct;
	t_command	*current_cmd;

	if (validate_token_syntax(tokens) == SYNTAX_ERROR)
		return (ft_putstr_fd("Error: syntax error\n", STDERR_FILENO),
			update_exit_code(cmd->shell, 2), 1);
	ct = tokens;
	current_cmd = cmd;
	if (process_command_tokens(ct, current_cmd, cmd) == 1)
		return (1);
	ft_function_marker(cmd);
	return (0);
}
