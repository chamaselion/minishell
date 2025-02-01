/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:57:15 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 15:57:36 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal;

void	handle_sigint(int sig)
{
	g_received_signal = sig;
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	g_received_signal = sig;
}

void	setup_signal_handling(void)
{
	struct sigaction	sa;

	g_received_signal = 0;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_putstr_fd("sigaction error for SIGINT\n", STDERR_FILENO);
		exit(1);
	}
	sa.sa_handler = handle_sigquit;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		ft_putstr_fd("sigaction error for SIGQUIT\n", STDERR_FILENO);
		exit(1);
	}
}
