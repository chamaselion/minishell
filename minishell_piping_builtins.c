/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_builtins.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:10:12 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 11:58:17 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	serialize_env_var(t_env_var *var, int pipe_fd)
{
	write(pipe_fd, var->string, ft_strlen(var->string) + 1);
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
			set_or_create_env_var(&shell->env_vars, key, value, 1);
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
	int			status;

	close(env_pipe[0]);
	if (setup_redirection(cmd, in_fd, pipefd))
		exit(1);
	status = handle_ft_command(cmd);
	current = cmd->shell->env_vars;
	while (current)
	{
		serialize_env_var(current, env_pipe[1]);
		current = current->next;
	}
	close(env_pipe[1]);
	restore_shell_fds(cmd->shell);
	exit(status);
}

int	execute_builtin(t_command *cmd)
{
	int	status;

	save_shell_fds(cmd->shell);
	if (setup_redirection(cmd, 0, NULL) == 1)
		return (restore_shell_fds(cmd->shell), 1);
	status = handle_ft_command(cmd);
	restore_shell_fds(cmd->shell);
	return (status);
}
