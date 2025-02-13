/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_piping_utis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:11:26 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 16:37:16 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_code(t_shell *shell, int exit_code)
{
	shell->last_exit_code = exit_code;
}

char	*get_exit_code_str(t_shell *shell)
{
	char	*str;

	str = ft_itoa(shell->last_exit_code);
	if (!str)
		return (NULL);
	return (str);
}

void	restore_shell_fds(t_shell *shell)
{
	if (shell->saved_stdin != -1)
	{
		dup2(shell->saved_stdin, STDIN_FILENO);
		close(shell->saved_stdin);
	}
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT_FILENO);
		close(shell->saved_stdout);
	}
	if (shell->saved_stderr != -1)
	{
		dup2(shell->saved_stderr, STDERR_FILENO);
		close(shell->saved_stderr);
	}
}

void	save_shell_fds(t_shell *shell)
{
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	shell->saved_stderr = dup(STDERR_FILENO);
}
