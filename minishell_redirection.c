/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:01:20 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/24 00:10:09 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->input_redirection)
	{
		fd = open(cmd->input_redirection->content, O_RDONLY);
		if (fd == -1)
		{
			perror("open input redirection");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input redirection");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

void	handle_output_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->output_redirection)
	{
		fd = open(cmd->output_redirection->content,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open output redirection");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output redirection");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

void	handle_append_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->append_redirection)
	{
		fd = open(cmd->append_redirection->content,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open append redirection");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 append redirection");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

void	handle_pipe_redirection(t_command *cmd, int pipefd[2])
{
	if (cmd->related_to != NULL && cmd->relation_type == 6)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

void	handle_heredoc_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->heredoc_redirection)
	{
		fd = open(cmd->heredoc_redirection->content, O_RDONLY);
		if (fd == -1)
		{
			perror("open heredoc redirection");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc redirection");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}
