/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free_and_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:09:32 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/26 17:57:29 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	free_command(t_command *cmd)
{
	if (cmd->command)
		free(cmd->command);
	if (cmd->input)
		free(cmd->input);
	if (cmd->args)
	{
		for (int i = 0; i < cmd->arg_count; i++)
		{
			if (cmd->args[i])
				free(cmd->args[i]);
		}
		free(cmd->args);
	}
	if (cmd->output)
		cmd->output = NULL;
	if (cmd->input_redirection)
		cmd->input_redirection = NULL;
	if (cmd->output_redirection)
		cmd->output_redirection = NULL;
	if (cmd->append_redirection)
		cmd->append_redirection = NULL;
	if (cmd->heredoc_redirection)
		cmd->heredoc_redirection = NULL;
}

void	free_commands(t_command *cmd)
{
	t_command	*temp;

	while (cmd != NULL)
	{
		temp = cmd;
		cmd = cmd->next;
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

void free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;

    if (!tokens)
        return;
    current = tokens;
    while (current) {
        next = current->next;
        if (current->content)
            free(current->content);
        free(current);
        current = next;
    }
}

