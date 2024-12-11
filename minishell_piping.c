/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:26:56 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 12:31:42 by bszikora         ###   ########.fr       */
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
    if (cmd->is_internal)
    {
        handle_ft_command(cmd);
        exit(0);
    }
    else
        execute_command(cmd, exec_args);
}

void handle_parent_process(t_command *cmd, int *in_fd, int pipefd[2])
{
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: cmd is NULL in handle_parent_process\n");
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
}

void handle_pipes(t_command *cmd)
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
}
