/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:24:59 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 12:32:08 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void handle_ft_command(t_command *cmd)
{
	(void)cmd;
   /* if (strcmp(cmd->command, "cd") == 0)
        ft_cd(cmd);
	if (strcmp(cmd->command, "echo") == 0)
        ft_echo(cmd);
	if (strcmp(cmd->command, "exit") == 0)
        ft_exit(cmd);
	if (strcmp(cmd->command, "export") == 0)
        ft_export(cmd);
	if (strcmp(cmd->command, "pwd") == 0)
        ft_pwd(cmd);*/
}

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
	path = strdup(path_env);
	if (!path)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	dir = strtok(path, ":");
	while (dir != NULL)
	{
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (result = strdup(full_path), free(path), result);
		dir = strtok(NULL, ":");
	}
	return (free(path), NULL);
}

char **construct_exec_args(t_command *cmd)
{
    char **exec_args;
	int c;

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
    return exec_args;
}

void execute_command(t_command *cmd, char **exec_args)
{
    char *full_path;
	
	full_path = search_command(cmd->command);
    if (full_path)
    {
        execve(full_path, exec_args, NULL);
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