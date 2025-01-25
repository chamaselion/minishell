/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:24:59 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/25 23:57:07 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	handle_ft_command(t_command *cmd)
{
	int	status;

	status = 0;
	if (!cmd || !cmd->command)
		return ;
	if (ft_strcmp(cmd->command, "cd") == 0)
		status = ft_cd(cmd);
	else if (ft_strcmp(cmd->command, "echo") == 0)
		status = ft_echo(cmd);
	else if (ft_strcmp(cmd->command, "export") == 0)
		status = ft_export(cmd);
	else if (ft_strcmp(cmd->command, "pwd") == 0)
		status = ft_pwd();
	else if (ft_strcmp(cmd->command, "env") == 0)
		status = ft_env(cmd);
	else if (ft_strcmp(cmd->command, "unset") == 0)
		status = ft_unset(cmd);
	else if (ft_strcmp(cmd->command, "exit") == 0)
		status = ft_exit(cmd);
	update_exit_code(cmd->shell, status);
	return ;
}

char	*ft_getenv(t_env_var *env_vars, const char *name)
{
	t_env_var	*current;

	if (!env_vars || !name)
		return (NULL);
	current = env_vars;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*find_executable_in_path(const char *command, char *path,
		char *full_path)
{
	char	*dir;
	char	*result;

	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		ft_strcpy(full_path, dir);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (result = ft_strdup(full_path), free(path), result);
		dir = ft_strtok(NULL, ":");
	}
	return (free(path), NULL);
}

char	*search_command(const char *command, t_env_var *env_var)
{
	char	*path;
	char	full_path[4096];
	char	*path_env;
	char	*result;

	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = ft_getenv(env_var, "PATH");
	if (!path_env)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	path = ft_strdup(path_env);
	if (!path)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	result = find_executable_in_path(command, path, full_path);
	return (result);
}

char	**construct_exec_args(t_command *cmd)
{
	char	**exec_args;
	int		c;

	c = 0;
	exec_args = malloc((cmd->arg_count + 2) * sizeof(char *));
	if (!exec_args)
	{
		ft_putstr_fd("Error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	exec_args[0] = cmd->command;
	while (c < cmd->arg_count)
	{
		exec_args[c + 1] = cmd->args[c];
		c++;
	}
	exec_args[cmd->arg_count + 1] = NULL;
	return (exec_args);
}

void	execute_command(t_command *cmd, char **exec_args)
{
	char	*full_path;
	char	**environment;

	full_path = search_command(cmd->command, cmd->shell->env_vars);
	if (!full_path)
	{
		ft_putstr_fd(cmd->command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (free(exec_args), exit(127));
	}
	if (access(full_path, X_OK) != 0)
	{
		ft_putstr_fd(cmd->command, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free(full_path);
		free(exec_args);
		exit(126);
	}
	environment = convertEnvironmentToArray(cmd->shell->env_vars);
	execve(full_path, exec_args, environment);
	free_split_array(environment);
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(full_path, STDERR_FILENO);
	ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	return (free(full_path), free(exec_args), exit(126));
}
