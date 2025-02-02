/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:06:43 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:06:43 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(const char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (input == NULL)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(0);
	}
	add_history(input);
	return (input);
}

void	shell_to_command(t_command **cmd, t_shell *shell)
{
	t_command	*current_cmd;

	current_cmd = *cmd;
	while (current_cmd)
	{
		current_cmd->shell = shell;
		current_cmd = current_cmd->next;
	}
}

int	sigint_checker(int original)
{
	if (g_received_signal == SIGINT)
		return (g_received_signal = 0, 130);
	else
		return (original);
}

int	main_loop(t_shell *shell)
{
	char		*input;
	t_raw_token	*raw_tokens;
	t_token		*tokens;
	t_command	*commands;

	commands = NULL;
	while (1)
	{
		signal_interactive();
		shell->last_exit_code = sigint_checker(shell->last_exit_code);
		input = read_input("minishell> ");
		raw_tokens = handle_input(input, shell);
		if (check_for_unclosed(raw_tokens, shell) == 0)
		{
			tokens = convert_raw_token_list(raw_tokens);
			process_tokens_and_commands(tokens, shell);
			free_tokens(tokens);
			free(input);
		}
		else
			free_raw_and_input(raw_tokens, input);
	}
	free_shell(shell);
	rl_clear_history();
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int			i;
	t_env_var	*env_vars;
	t_shell		shell;

	(void)argc;
	(void)argv;
	env_vars = init_env_vars(envp);
	init_shell(&shell, env_vars);
	i = main_loop(&shell);
	if (i > 0)
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
	}
	return (0);
}
