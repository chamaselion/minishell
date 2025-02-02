/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_syntax_check.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 00:18:22 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/24 00:18:22 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_token_syntax(t_token *token_list)
{
	t_token *current;

	current = token_list;
	while (current != NULL)
	{
		if (current->role == ROLE_PIPE)
		{
			if (!current->prev || !(current->prev->content))
				return (SYNTAX_ERROR);
			if (current->prev->role == ROLE_PIPE)
				return (SYNTAX_ERROR);
		}
		if (current->role == ROLE_REDIRECT)
		{
			if (!current->next || !(current->next->content))
				return (SYNTAX_ERROR);
			if (current->next->role != ROLE_ARGUMENT)
				return (SYNTAX_ERROR);
		}
		current = current->next;
	}
	return (SYNTAX_VALID);
}
