/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:18:46 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/30 00:29:18 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

char	*get_prompt(void)
{
	char		*cwd;
	char		*prompt;
	const char	*suffix;

	suffix = " minishell> ";
	cwd = malloc(2048);
	if (cwd == NULL)
		return (ft_putstr_fd("malloc() error", STDERR_FILENO), NULL);
	if (getcwd(cwd, 2048) == NULL)
	{
		ft_putstr_fd("getcwd() error", STDERR_FILENO);
		return (free(cwd), NULL);
	}
	prompt = (char *)malloc(ft_strlen(cwd) + ft_strlen(suffix) + 1);
	if (prompt == NULL)
	{
		ft_putstr_fd("malloc() error", STDERR_FILENO);
		free(cwd);
		return (NULL);
	}
	ft_strcpy(prompt, cwd);
	return (ft_strcat(prompt, suffix), free(cwd), prompt);
}

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
	char		*prompt;
	char		*input;
	t_raw_token	*raw_tokens;
	t_token		*tokens;	
	t_command	*commands;

	setup_signal_handling();
	while (1)
	{
		shell->last_exit_code = sigint_checker(shell->last_exit_code);
		prompt = get_prompt();
		if (prompt == NULL)
			return (1);
		input = read_input(prompt);
		free(prompt);
		raw_tokens = handle_input(input, shell);
		//check_for_unclosed(raw_tokens, shell);
		tokens = convert_raw_token_list(raw_tokens);
		if (tokens)
		{
			if (fill_command_from_tokens(tokens, &commands) != -1)
			{
				shell_to_command(&commands, shell);
				if (link_commands_and_tokens(tokens, commands) == 0 && commands)
				{
					handle_pipes(commands);
					free_commands(commands);
				}
			}
		}
		free_tokens(tokens);
		free(input);
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
		ft_putstr_fd("Error", STDERR_FILENO);
	}
	return (0);
}
