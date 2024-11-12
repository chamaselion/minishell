/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:27:50 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/05 13:30:53 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_builtin_type(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export", 
							"unset", "env", "exit", NULL};
	int			i;

	i = 0;
	while (builtins[i])
	{
		if (!ft_strcmp(cmd, builtins[i]))
			return (i);
		i++;
	}
	return (-1);
}

static int	handle_command_token(t_token *token)
{
	int	builtin_type;

	builtin_type = get_builtin_type(token->start);
	if (builtin_type >= 0)
	{
		token->role = ROLE_BUILTIN;
		return (builtin_type);
	}
	if (search_command(token->start))
		token->role = ROLE_EXECUTABLE;
	else
		token->role = ROLE_ERROR;
	return (-1);
}

static int	handle_redirection(t_token *token)
{
	if (ft_strchr("<>|", token->start[0]))
	{
		token->role = ROLE_REDIRECT;
		if (token->start[0] == '|')
			token->role = ROLE_PIPE;
		return (1);
	}
	return (0);
}

static int	process_token_role(t_token *token, int *expect_cmd, int *is_echo)
{
	if (handle_redirection(token))
	{
		*expect_cmd = 1;
		return (1);
	}
	if (*expect_cmd)
	{
		*is_echo = (handle_command_token(token) == echo);
		*expect_cmd = 0;
		return (1);
	}
	if (*is_echo && !ft_strcmp(token->start, "-n"))
		token->role = ROLE_ARGUMENT;
	else if (token->start[0] == '$')
		token->role = ROLE_ASSIGNMENTOPERATOR;
	else
		token->role = ROLE_ARGUMENT;
	return (1);
}

int	assign_roles(t_parsed_input *parsed_input)
{
	t_token	*current;
	int		expect_cmd;
	int		is_echo;

	if (!process_quotes(parsed_input))
		return (0);
	current = parsed_input->token;
	expect_cmd = 1;
	is_echo = 0;
	while (current)
	{
		if (current->role != ROLE_DELIMITER)
			if (!process_token_role(current, &expect_cmd, &is_echo))
				return (0);
		current = current->next;
	}
	return (1);
}
