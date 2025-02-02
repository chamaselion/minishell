/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:01:09 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 15:03:27 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_syntax(t_token *current, t_token *prev)
{
	if (current->role == ROLE_PIPE)
	{
		if (!prev || !prev->content || (ft_strcmp(prev->content, "\"\"") == 0))
			return (SYNTAX_ERROR);
	}
	return (SYNTAX_VALID);
}

int	check_redirect_syntax(t_token *current)
{
	if (current->role == ROLE_REDIRECT)
	{
		if (!current->next || !current->next->content)
			return (SYNTAX_ERROR);
		if (current->next->role != ROLE_ARGUMENT)
			return (SYNTAX_ERROR);
		if (ft_strcmp(current->next->content, "\"\"") == 0)
			return (SYNTAX_ERROR);
	}
	return (SYNTAX_VALID);
}

int	validate_token_syntax(t_token *token_list)
{
	t_token	*current;
	t_token	*prev;

	current = token_list;
	prev = NULL;
	while (current != NULL)
	{
		if (check_pipe_syntax(current, prev) == SYNTAX_ERROR)
			return (SYNTAX_ERROR);
		if (check_redirect_syntax(current) == SYNTAX_ERROR)
			return (SYNTAX_ERROR);
		prev = current;
		current = current->next;
	}
	return (SYNTAX_VALID);
}
