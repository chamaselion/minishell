/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:24:59 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 15:55:38 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_command_access(t_command *cmd, char *full_path, char **exec_args)
{
	if (access(full_path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->command, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free(full_path);
		free(exec_args);
		exit(126);
	}
}

void	execute_external_command(t_command *cmd, char **exec_args)
{
	char	*full_path;
	char	**environment;

	full_path = search_command(cmd->command, cmd->shell->env_vars);
	if (!full_path)
	{
		ft_putstr_fd(cmd->command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free(exec_args);
		exit(127);
	}
	check_command_access(cmd, full_path, exec_args);
	environment = convert_environment_to_array(cmd->shell->env_vars);
	execve(full_path, exec_args, environment);
	free_split_array(environment);
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(cmd->command, STDERR_FILENO);
	ft_putstr_fd(": can be a directory, or faulty command.\n", STDERR_FILENO);
	free(full_path);
	free(exec_args);
	exit(126);
}

void	execute_command(t_command *cmd, char **exec_args)
{
	int	status;

	if (cmd->is_internal)
	{
		status = handle_ft_command(cmd);
		free(exec_args);
		exit(status);
	}
	execute_external_command(cmd, exec_args);
}
