/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:47:00 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/28 12:27:56 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mshell_exec.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, strlen(s));
}

void handle_ft_command(t_command *cmd)
{
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

void setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
    int fd;
	
    if (cmd->input_redirection)
	{
        fd = open(cmd->input_redirection->start, O_RDONLY);
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
	else if (in_fd != 0)
	{
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (cmd->output_redirection)
	{
        fd = open(cmd->output_redirection->start, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	else if (cmd->append_redirection)
	{
        fd = open(cmd->append_redirection->start, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
	else if (cmd->related_to != NULL && cmd->relation_type == 6)
	{
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    if (cmd->heredoc_redirection)
	{
        fd = open(cmd->heredoc_redirection->start, O_RDONLY);
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

void create_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}

pid_t fork_process()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    return pid;
}

void handle_child_process(t_command *cmd, int in_fd, int pipefd[2])
{
	char **exec_args;
    setup_redirection(cmd, in_fd, pipefd);
    if (cmd->is_internal)
    {
        handle_ft_command(cmd);
        exit(0);
    }
    else
    {
        exec_args = construct_exec_args(cmd);
        execute_command(cmd, exec_args);
    }
}

void handle_parent_process(t_command *cmd, int *in_fd, int pipefd[2])
{
    if (*in_fd != 0)
        close(*in_fd);
    if (cmd->relation_type == 6 && cmd->related_to != NULL)
    {
        close(pipefd[1]);
        *in_fd = pipefd[0];
    }
    else
        *in_fd = 0;
    waitpid(-1, NULL, 0);
}

void handle_pipes(t_command *cmd)
{
    int pipefd[2];
    pid_t pid;
    int in_fd = 0;

    while (cmd != NULL)
    {
        if (cmd->relation_type == 6 && cmd->related_to != NULL)
        {
            create_pipe(pipefd);
        }
        pid = fork_process();
        if (pid == 0)
        {
            handle_child_process(cmd, in_fd, pipefd);
        }
        else
        {
            handle_parent_process(cmd, &in_fd, pipefd);
        }
        cmd = cmd->related_to;
    }
}

int main()
{
    // Input redirection test
    t_command cmd1;
    t_token input_token;
    input_token.start = "input.txt";
    input_token.length = strlen(input_token.start);
    input_token.role = 5; // Assuming 5 is the role for redirection
    input_token.next = NULL;
    input_token.prev = NULL;

    cmd1.command = "cat";
    cmd1.args = (char *[]){NULL};
    cmd1.arg_count = 0;
    cmd1.input = NULL;
    cmd1.output = NULL;
    cmd1.related_to = NULL;
    cmd1.relation_type = 5;
    cmd1.next = NULL;
    cmd1.append = 0;
    cmd1.is_internal = 0;
    cmd1.input_redirection = &input_token;
    cmd1.output_redirection = NULL;
    cmd1.append_redirection = NULL;
    cmd1.heredoc_redirection = NULL;

    // Output redirection test
    t_command cmd2;
    t_token output_token;
    output_token.start = "output.txt";
    output_token.length = strlen(output_token.start);
    output_token.role = 5; // Assuming 5 is the role for redirection
    output_token.next = NULL;
    output_token.prev = NULL;

    cmd2.command = "echo";
    cmd2.args = (char *[]){"Hello, World!", NULL};
    cmd2.arg_count = 1;
    cmd2.input = NULL;
    cmd2.output = NULL;
    cmd2.related_to = NULL;
    cmd2.relation_type = 5;
    cmd2.next = NULL;
    cmd2.append = 0;
    cmd2.is_internal = 0;
    cmd2.input_redirection = NULL;
    cmd2.output_redirection = &output_token;
    cmd2.append_redirection = NULL;
    cmd2.heredoc_redirection = NULL;

    // Append redirection test
    t_command cmd3;
    t_token append_token;
    append_token.start = "append.txt";
    append_token.length = strlen(append_token.start);
    append_token.role = 5; // Assuming 5 is the role for redirection
    append_token.next = NULL;
    append_token.prev = NULL;

    cmd3.command = "echo";
    cmd3.args = (char *[]){"Appending text", NULL};
    cmd3.arg_count = 1;
    cmd3.input = NULL;
    cmd3.output = NULL;
    cmd3.related_to = NULL;
    cmd3.relation_type = 5;
    cmd3.next = NULL;
    cmd3.append = 1;
    cmd3.is_internal = 0;
    cmd3.input_redirection = NULL;
    cmd3.output_redirection = NULL;
    cmd3.append_redirection = &append_token;
    cmd3.heredoc_redirection = NULL;

    // Heredoc redirection test
    t_command cmd4;
    t_token heredoc_token;
    heredoc_token.start = "heredoc.txt"; // Assuming heredoc content is stored in a file
    heredoc_token.length = strlen(heredoc_token.start);
    heredoc_token.role = 5; // Assuming 5 is the role for redirection
    heredoc_token.next = NULL;
    heredoc_token.prev = NULL;

    cmd4.command = "cat";
    cmd4.args = (char *[]){NULL};
    cmd4.arg_count = 0;
    cmd4.input = NULL;
    cmd4.output = NULL;
    cmd4.related_to = NULL;
    cmd4.relation_type = 5;
    cmd4.next = NULL;
    cmd4.append = 0;
    cmd4.is_internal = 0;
    cmd4.input_redirection = NULL;
    cmd4.output_redirection = NULL;
    cmd4.append_redirection = NULL;
    cmd4.heredoc_redirection = &heredoc_token;

    // Execute commands
    handle_pipes(&cmd1);
    handle_pipes(&cmd2);
    handle_pipes(&cmd3);
    handle_pipes(&cmd4);

    return 0;
}