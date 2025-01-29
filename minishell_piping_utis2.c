/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_utis2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:15:26 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 16:37:19 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
	save_shell_fds(cmd->shell);
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

void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("Error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
