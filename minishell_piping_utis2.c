/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_utis2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/01/29 15:15:26 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 22:35:38 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_redir_handlers(int (**redir_handlers)(t_command *))
{
	redir_handlers[0] = NULL;
	redir_handlers[1] = handle_output_redirection;
	redir_handlers[2] = handle_input_redirection;
	redir_handlers[3] = handle_append_redirection;
	redir_handlers[4] = handle_heredoc_redirection;
}

int	setup_redirection(t_command *cmd, int in_fd, int pipefd[2])
{
	int	(*redir_handlers[5])(t_command *);
	int	i;
	int	redir_type;

	save_shell_fds(cmd->shell);
	initialize_redir_handlers(redir_handlers);
	if (!cmd->input_redirections && in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	i = 0;
	while (i < cmd->redir_count)
	{
		redir_type = cmd->redir_order[i];
		if (redir_type > 0 && redir_type <= 4 && redir_handlers[redir_type])
		{
			if (redir_handlers[redir_type](cmd) == 1)
				return (1);
		}
		i++;
	}
	if (!cmd->output_redirections && !cmd->append_redirections)
		handle_pipe_redirection(cmd, pipefd);
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
