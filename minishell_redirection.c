/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:01:20 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 23:03:24 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->input_redirection)
	{
		if (access(cmd->input_redirection->content, F_OK) == -1)
		{
			ft_putstr_fd("Error: No such file or directory\n", STDERR_FILENO);
			return (1);
		}
		if (access(cmd->input_redirection->content, R_OK) == -1)
		{
			ft_putstr_fd("Error: Permission denied\n", STDERR_FILENO);
			return (1);
		}
		fd = open(cmd->input_redirection->content, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("open input redirection", STDERR_FILENO);
			return (1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("dup2 input redirection", STDERR_FILENO);
			return (1);
		}
		close(fd);
	}
	return (0);
}

int	handle_output_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->output_redirection)
	{
		if (access(cmd->output_redirection->content, F_OK) != -1
			&& access(cmd->output_redirection->content, W_OK) == -1)
		{
			ft_putstr_fd("Error: Permission denied\n", STDERR_FILENO);
			return (1);
		}
		fd = open(cmd->output_redirection->content,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("Error creating/opening file\n", STDERR_FILENO);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("Error: Failed to redirect output\n", STDERR_FILENO);
			return (1);
		}
		close(fd);
	}
	return (0);
}

int	handle_append_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->append_redirection)
	{
		if (access(cmd->append_redirection->content, F_OK) != -1
			&& access(cmd->append_redirection->content, W_OK) == -1)
		{
			ft_putstr_fd("Error: Permission denied\n", STDERR_FILENO);
			return (1);
		}
		fd = open(cmd->append_redirection->content,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("Error creating/opening file\n", STDERR_FILENO);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("Error: Failed to redirect output\n", STDERR_FILENO);
			return (1);
		}
		close(fd);
	}
	return (0);
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

int	handle_heredoc_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->heredoc_redirection)
	{
		if (access(cmd->heredoc_redirection->content, F_OK) == -1)
		{
			ft_putstr_fd("Error: No such heredoc file\n", STDERR_FILENO);
			return (1);
		}
		if (access(cmd->heredoc_redirection->content, R_OK) == -1)
		{
			ft_putstr_fd("Error: Permission denied\n", STDERR_FILENO);
			return (1);
		}
		fd = open(cmd->heredoc_redirection->content, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("Error opening heredoc file\n", STDERR_FILENO);
			return (1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("Error: Failed to redirect heredoc\n", STDERR_FILENO);
			return (1);
		}
		close(fd);
	}
	return (0);
}
