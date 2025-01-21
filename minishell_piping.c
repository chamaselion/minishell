/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:26:56 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/21 14:17:51 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
    handle_input_redirection(cmd);
    if (!cmd->input_redirection && in_fd != 0)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    handle_output_redirection(cmd);
    handle_append_redirection(cmd);
    handle_pipe_redirection(cmd, pipefd);
    handle_heredoc_redirection(cmd);
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
    if (cmd == NULL)
    {
        ft_putstr_fd("Error, command is NULL in handle_child_process", STDERR_FILENO);
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
/*
void handle_child_process(t_command *cmd, int in_fd, int pipefd[2])
{
	char **exec_args;
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: cmd is NULL in handle_child_process\n");
        exit(EXIT_FAILURE);
    }
    exec_args = construct_exec_args(cmd);
    if (exec_args == NULL)
    {
        fprintf(stderr, "Error: Failed to construct exec args\n");
        exit(EXIT_FAILURE);
    }
    setup_redirection(cmd, in_fd, pipefd);
    if (cmd->is_internal == 1)
    {
        handle_ft_command(cmd);
        exit(0);
    }
    else
        execute_command(cmd, exec_args);
}*/

void update_exit_code(t_shell *shell, int exit_code)
{
    shell->last_exit_code = exit_code;
}

char *get_exit_code_str(t_shell *shell)
{
    char *str;
    
    str = ft_itoa(shell->last_exit_code);
    if (!str)
        return (NULL);
    return (str);
}

void handle_parent_process(t_command *cmd, int *in_fd, int pipefd[2])
{
    if (cmd == NULL)
    {
        ft_putstr_fd("Error, command is NULL in handle_parent_process", STDERR_FILENO);
        return;
    }
    int status;
    if (*in_fd != 0)
        close(*in_fd);
    if (cmd->relation_type == 6 && cmd->related_to != NULL)
    {
        close(pipefd[1]);
        *in_fd = pipefd[0];
    }
    else
    {
        *in_fd = 0;
    }
    waitpid(-1, &status, 0);
	if (WIFEXITED(status))
        update_exit_code(cmd->shell, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        update_exit_code(cmd->shell, 128 + WTERMSIG(status));
}

void execute_builtin(t_command *cmd)
{
    setup_redirection(cmd, 0, NULL);
    handle_ft_command(cmd);
}

/*
void execute_builtin_with_pipes(t_command *cmd, int in_fd, int pipefd[2])
{
    pid_t pid;

    if (cmd->relation_type == 6 || in_fd != 0) {
        // Need to fork for piped builtins
        pid = fork_process();
        if (pid == 0) {
            // Child process
            setup_redirection(cmd, in_fd, pipefd);
            handle_ft_command(cmd);
            exit(0);
        }
    } else {
        // Direct execution for non-piped builtins
        setup_redirection(cmd, 0, NULL);
        handle_ft_command(cmd);
    }
}*/

void serialize_env_var(t_env_var *var, int pipe_fd)
{
    write(pipe_fd, var->string, strlen(var->string) + 1);
}

void deserialize_and_update_env(t_shell *shell, int pipe_fd)
{
    char buffer[4096];
    int bytes_read;
    char *key;
    char *value;
    char *equal_sign;

    while ((bytes_read = read(pipe_fd, buffer, sizeof(buffer))) > 0)
    {
        equal_sign = strchr(buffer, '=');
        if (equal_sign)
        {
            key = strndup(buffer, equal_sign - buffer);
            value = strdup(equal_sign + 1);
            set_or_create_env_var(&shell->env_vars, key, value);
            free(key);
            free(value);
        }
    }
}

void execute_builtin_with_pipes(t_command *cmd, int in_fd, int pipefd[2])
{
    int env_pipe[2];
    pid_t pid;
	t_env_var *current;

    if (pipe(env_pipe) == -1)
    {
        perror("pipe");
        return;
    }
    if (cmd->relation_type == 6 || in_fd != 0)
	{
        pid = fork_process();
        if (pid == 0)
		{
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
            exit(0);
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
    }
}

void handle_pipes(t_command *cmd)
{
    int pipefd[2];
    pid_t pid;
    int in_fd = 0;

    if (cmd == NULL)
    {
        ft_putstr_fd("Error, cmd NULL in pipes\n", STDERR_FILENO);
        return;
    }
    while (cmd != NULL)
    {
        if (cmd->relation_type == 6 && cmd->related_to != NULL)
            create_pipe(pipefd);
        if (cmd->is_internal)
        {
            execute_builtin_with_pipes(cmd, in_fd, pipefd);
            if (cmd->relation_type == 6)
			{
                close(pipefd[1]);
                in_fd = pipefd[0];
            }
        }
        else
        {
            pid = fork_process();
            if (pid == 0)
                handle_child_process(cmd, in_fd, pipefd);
            else
                handle_parent_process(cmd, &in_fd, pipefd);
        }
        cmd = cmd->related_to;
    }
    while (wait(NULL) > 0);
}
/*
void handle_pipes(t_command *cmd) VERSION 1
{
    int pipefd[2];
    pid_t pid;
    int in_fd = 0;

    if (cmd == NULL)
    {
        fprintf(stderr, "Error: cmd is NULL in handle_pipes\n");
        return;
	}
    while (cmd != NULL)
    {
        if (cmd->relation_type == 6 && cmd->related_to != NULL)
            create_pipe(pipefd);
        pid = fork_process();
        if (pid == 0)
            handle_child_process(cmd, in_fd, pipefd);
        else
            handle_parent_process(cmd, &in_fd, pipefd);
        cmd = cmd->related_to;
    }
}*/
