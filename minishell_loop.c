/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:18:46 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/10 14:54:16 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

char *get_prompt() // Function to get the current working directory address and combine with the prefix we want
{
	char *cwd;
	char *prompt;
	const char *suffix;

	suffix = " minishell> ";
	cwd = malloc(2048);
	if (cwd == NULL)
	{
		ft_putstr_fd("malloc() error", STDERR_FILENO);
		return NULL;
	}
	if (getcwd(cwd, 2048) == NULL)
	{
		ft_putstr_fd("getcwd() error", STDERR_FILENO);
		free(cwd);
		return NULL;
	}
	prompt = (char *)malloc(ft_strlen(cwd) + ft_strlen(suffix) + 1);
	if (prompt == NULL)
	{
		ft_putstr_fd("malloc() error", STDERR_FILENO);
		free(cwd);
		return NULL;
	}
	return (strcpy(prompt, cwd), strcat(prompt, suffix), free(cwd), prompt); // Need to implement strcpy, strcat
}

char *read_input(const char *prompt) // Getting the line from the user, adding it to history
{
	char *input = readline(prompt);
	if (input == NULL)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(0);
	}
	add_history(input);
	return input;
}

int main_loop(void) // Function with the loop to keep looking for inputs, I tried to keep it as small as possible,
					// as I think it will contain lots of stuff like handling signals, getting values etc..
					//'CTRL + D' sets the prompt to NULL, so it is handled here
{
	char *prompt;
	char *input;

	setup_signal_handling();

	while (1)
	{
		prompt = get_prompt();
		if (prompt == NULL)
			return 1;
		input = read_input(prompt);
		free(prompt);
		if (handle_input(input))
			return 1;
	}
	return 0;
}

int main(void)
{
	int i;

	i = main_loop();
	if (i > 0)
	{
		ft_putstr_fd("Error", STDERR_FILENO);
	}
	return 0;
}