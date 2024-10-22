/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:47:00 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/22 11:21:04 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mshell_exec.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, strlen(s));
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

void setup_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}

void fork_process(pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}

void setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
    if (in_fd != 0)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (cmd->related_to != NULL && cmd->relation_type == 6)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
}

char **construct_exec_args(t_command *cmd)
{
    char **exec_args;
	int i;

	i = 0;
	exec_args = malloc((cmd->arg_count + 2) * sizeof(char *));
    if (!exec_args)
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    exec_args[0] = cmd->command;
    for (int i = 0; i < cmd->arg_count; i++)
    {
        exec_args[i + 1] = cmd->args[i];
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

void handle_child_process(t_command *cmd, int in_fd, int pipefd[2])
{
	char **exec_args;

    setup_redirection(cmd, in_fd, pipefd);
    exec_args = construct_exec_args(cmd);
    execute_command(cmd, exec_args);
}

void handle_parent_process(pid_t pid, int *in_fd, int pipefd[2])
{
    int status;
    waitpid(pid, &status, 0);
    if (*in_fd != 0)
        close(*in_fd);
    close(pipefd[1]);
    *in_fd = pipefd[0];
}

void handle_pipes(t_command *cmd)
{
    int pipefd[2];
    pid_t pid;
    int in_fd;
	
	in_fd = 0;
    while (cmd != NULL)
    {
        if (cmd->relation_type != 6)
        {
            cmd = cmd->next;
            continue;
        }
        if (cmd->related_to != NULL && cmd->relation_type == 6)
            setup_pipe(pipefd);
        fork_process(&pid);
        if (pid == 0)
            handle_child_process(cmd, in_fd, pipefd);
        else
            handle_parent_process(pid, &in_fd, pipefd);
        cmd = cmd->next;
    }
}

int main()
{
	t_command cmd1;
	cmd1.command = "who";
	cmd1.args = (char *[]){NULL}; // Exclude the command itself
	cmd1.arg_count = 0;
	cmd1.input = NULL;
	cmd1.output = NULL;
	cmd1.related_to = NULL;
	cmd1.relation_type = 6;
	cmd1.next = NULL;
	cmd1.append = 0;
	cmd1.priority = 0;
	t_command cmd2;
	cmd2.command = "wc";
	cmd2.args = (char *[]){"-l", NULL}; // Exclude the command itself
	cmd2.arg_count = 1;
	cmd2.input = NULL;
	cmd2.output = NULL;
	cmd2.related_to = NULL; // Set related command
	cmd2.relation_type = 6; // Pipe
	cmd2.next = NULL;
	cmd2.append = 0;
	cmd2.priority = 0;

	cmd1.next = &cmd2;
	cmd1.related_to = &cmd2;
	printf("Executing commands with pipe:\n");
	handle_pipes(&cmd1);
	printf("done");

	return 0;
}
