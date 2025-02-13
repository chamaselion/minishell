/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free_and_exit2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:16:20 by mnaumann          #+#    #+#             */
/*   Updated: 2025/02/02 21:23:58 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_raw_tokens(t_raw_token *first_token)
{
	t_raw_token	*temp;

	temp = NULL;
	while (first_token != NULL)
	{
		temp = first_token;
		first_token = first_token->next;
		if (temp->segment)
			free(temp->segment);
		free(temp);
	}
}

void	free_env_vars(t_env_var *env_vars)
{
	t_env_var	*current;
	t_env_var	*next;

	current = env_vars;
	while (current)
	{
		next = current->next;
		free(current->string);
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env_vars)
		free_env_vars(shell->env_vars);
}

void	free_raw_and_input(t_raw_token *raw_tokens, char *input)
{
	free_raw_tokens(raw_tokens);
	free(input);
}
