/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:26:56 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 16:37:21 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fork_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	handle_child_process(t_command *cmd, int in_fd, int pipefd[2])
{
	char	**exec_args;

	if (cmd == NULL)
	{
		ft_putstr_fd("Error, command is NULL in handle_child_process",
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	exec_args = construct_exec_args(cmd);
	if (exec_args == NULL)
	{
		ft_putstr_fd("Error, failed to construct exec args", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	setup_redirection(cmd, in_fd, pipefd);
	execute_command(cmd, exec_args);
}

void	handle_parent_process(t_command *cmd, int *in_fd, int pipefd[2])
{
	int		status;
	pid_t	wait_result;

	if (cmd == NULL)
	{
		ft_putstr_fd("Error, command is NULL in handle_parent_process",
			STDERR_FILENO);
		return ;
	}
	if (*in_fd != 0)
		close(*in_fd);
	if (cmd->relation_type == 6 && cmd->related_to != NULL)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	else
		*in_fd = 0;
	wait_result = waitpid(-1, &status, 0);
	if (wait_result == -1)
		return ;
	if ((status & 0x7F) == 0)
		update_exit_code(cmd->shell, (status >> 8) & 0xFF);
	else
		update_exit_code(cmd->shell, 128 + (status & 0x7F));
}

void	process_command(t_command *cmd, int *in_fd)
{
	int		pipefd[2];
	pid_t	pid;

	if (cmd->relation_type == 6 && cmd->related_to != NULL)
		create_pipe(pipefd);
	if (cmd->is_internal)
	{
		execute_builtin_with_pipes(cmd, *in_fd, pipefd);
		if (cmd->relation_type == 6)
		{
			close(pipefd[1]);
			*in_fd = pipefd[0];
		}
	}
	else
	{
		pid = fork_process();
		if (pid == 0)
			handle_child_process(cmd, *in_fd, pipefd);
		else
			handle_parent_process(cmd, in_fd, pipefd);
	}
}

void	handle_pipes(t_command *cmd)
{
	int	in_fd;

	in_fd = 0;
	if (cmd == NULL)
	{
		ft_putstr_fd("Error, cmd NULL in pipes\n", STDERR_FILENO);
		return ;
	}
	while (cmd != NULL)
	{
		process_command(cmd, &in_fd);
		cmd = cmd->related_to;
	}
	while (wait(NULL) > 0)
		;
}
