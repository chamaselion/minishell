/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_executables.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:47:01 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/24 13:44:07 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

char *search_command(const char *command)
{
	char *path;
	char *dir;
	char full_path[4096];
	char *result;
	char *path_env;
	
	path_env = getenv("PATH");
	if (!path_env)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	path = ft_strdup(path_env);
	if (!path)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
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

char **construct_exec_args(t_command *cmd)
{
    char **exec_args;
	int i;
	int c;
	
	c = 0;
	i = 0;
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

void execute_command(t_command *cmd, char **exec_args)
{
    char *full_path;
	char **environment;
    
	full_path = search_command(cmd->command);
    if (full_path)
    {
        environment = convertEnvironmentToArray(cmd->shell->env_vars);
        execve(full_path, exec_args, environment);
        free_split_array(environment);
        ft_putstr_fd("Error", STDERR_FILENO);
        free(full_path);
        free(exec_args);
        exit(EXIT_FAILURE);
    }
    else
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        free(exec_args);
        exit(EXIT_FAILURE);
    }
}
