/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:47:05 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/28 12:55:45 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

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
    return (pid);
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
            create_pipe(pipefd);
        pid = fork_process();
        if (pid == 0)
            handle_child_process(cmd, in_fd, pipefd);
        else
            handle_parent_process(cmd, &in_fd, pipefd);
        cmd = cmd->related_to;
    }
}