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
	int (*redir_handlers[])(t_command *) = {
		NULL,                      // 0: none
		handle_output_redirection, // 1: output
		handle_input_redirection,  // 2: input
		handle_append_redirection, // 3: append
		handle_heredoc_redirection // 4: heredoc
	};

	if (!cmd->input_redirections && in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	
    for (int i = 0; i < cmd->redir_count; i++)
    {
        int redir_type = cmd->redir_order[i];
        if (redir_type > 0 && redir_type <= 4 && redir_handlers[redir_type])
        {
            if (redir_handlers[redir_type](cmd) == 1)
                return (1);
        }
    }

	if (!cmd->output_redirections && !cmd->append_redirections)
	{
		handle_pipe_redirection(cmd, pipefd);
	}
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
