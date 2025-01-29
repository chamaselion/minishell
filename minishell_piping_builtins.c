/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_builtins.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:10:12 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 16:37:14 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	serialize_env_var(t_env_var *var, int pipe_fd)
{
	write(pipe_fd, var->string, strlen(var->string) + 1);
}

void	deserialize_and_update_env(t_shell *shell, int pipe_fd)
{
	char	buffer[4096];
	int		bytes_read;
	char	*key;
	char	*value;
	char	*equal_sign;

	bytes_read = read(pipe_fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		equal_sign = ft_strchr(buffer, '=');
		if (equal_sign)
		{
			key = ft_strndup(buffer, equal_sign - buffer);
			value = ft_strdup(equal_sign + 1);
			set_or_create_env_var(&shell->env_vars, key, value);
			free(key);
			free(value);
		}
		bytes_read = read(pipe_fd, buffer, sizeof(buffer));
	}
}

void	execute_builtin_child_process(t_command *cmd, int in_fd, int pipefd[2],
		int env_pipe[2])
{
	t_env_var	*current;

	close(env_pipe[0]);
	setup_redirection(cmd, in_fd, pipefd);
	handle_ft_command(cmd);
	current = cmd->shell->env_vars;
	while (current)
	{
		serialize_env_var(current, env_pipe[1]);
		current = current->next;
	}
	close(env_pipe[1]);
	restore_shell_fds(cmd->shell);
	exit(0);
}

void	execute_builtin_with_pipes(t_command *cmd, int in_fd, int pipefd[2])
{
	int		env_pipe[2];
	pid_t	pid;

	if (pipe(env_pipe) == -1)
		return ;
	if (cmd->relation_type == 6 || in_fd != 0)
	{
		pid = fork_process();
		if (pid == 0)
		{
			execute_builtin_child_process(cmd, in_fd, pipefd, env_pipe);
		}
		else
		{
			close(env_pipe[1]);
			deserialize_and_update_env(cmd->shell, env_pipe[0]);
			close(env_pipe[0]);
		}
	}
	else
	{
		setup_redirection(cmd, 0, NULL);
		handle_ft_command(cmd);
		restore_shell_fds(cmd->shell);
	}
}
