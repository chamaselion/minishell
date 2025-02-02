/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free_and_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:05:22 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:05:22 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->command)
		free(cmd->command);
	if (cmd->input)
		free(cmd->input);
	if (cmd->args)
	{
		while (i < cmd->arg_count)
		{
			if (cmd->args[i])
				free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->output_redirections)
		free_redirect_list(cmd->output_redirections);
	if (cmd->input_redirections)
		free_redirect_list(cmd->input_redirections);
	if (cmd->append_redirections)
		free_redirect_list(cmd->append_redirections);
	if (cmd->heredoc_redirections)
		free_redirect_list(cmd->heredoc_redirections);
}

void	free_commands(t_command *cmd)
{
	t_command	*temp;

	while (cmd != NULL)
	{
		temp = cmd;
		cmd = cmd->next;
		if (temp->output)
			temp->output = NULL;
		free_command(temp);
		free(temp);
	}
}

int	free_and_return(char *content, int ret)
{
	free(content);
	return (ret);
}

int	free_and_fail(char *content, char *message)
{
	free(content);
	ft_putstr_fd(message, STDERR_FILENO);
	return (0);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->content)
			free(tmp->content);
		free(tmp);
		tmp = NULL;
	}
}
