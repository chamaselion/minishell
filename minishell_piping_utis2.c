/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_utis2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:15:26 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 22:35:38 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
	save_shell_fds(cmd->shell);
	if (handle_input_redirection(cmd) == 1)
		return (1);
	if (!cmd->input_redirections && in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (handle_output_redirection(cmd) == 1)
		return (1);
	if (handle_append_redirection(cmd) == 1)
		return (1);
    if (!cmd->output_redirections && !cmd->append_redirections)
	{
        handle_pipe_redirection(cmd, pipefd);
	}
	if (handle_heredoc_redirection(cmd) == 1)
		return (1);
	return (0);
}

void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("Error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
