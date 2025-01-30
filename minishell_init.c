/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:57:00 by root              #+#    #+#             */
/*   Updated: 2025/01/21 14:06:45 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	init_shell(t_shell *shell, t_env_var *env_vars)
{
	shell->commands = NULL;
	shell->env_vars = env_vars;
	shell->last_exit_code = 0;
	if (getcwd(shell->cwd, MAX_PATH) == NULL)
		ft_putstr_fd("getcwd() error", STDERR_FILENO);
}

void	init_token(t_token *token)
{
	token->next = NULL;
	token->prev = NULL;
	token->role = ROLE_DEFAULT;
	token->syntax_state = SYNTAX_VALID;
	token->separated = 0;
	token->command_expected = 0;
	token->quote_state = 0;
}

t_raw_list	init_raw_list(void)
{
	t_raw_list	raw_list;

	raw_list.first = NULL;
	raw_list.last = NULL;
	return (raw_list);
}
