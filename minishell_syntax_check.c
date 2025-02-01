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
		if (current->role == ROLE_PIPE && (!current->prev || !current->next
				|| current->prev->role == ROLE_PIPE))
		{
			return (SYNTAX_ERROR);
		}
		if (current->role == ROLE_REDIRECT && (!current->next
				|| current->next->role == ROLE_REDIRECT
				|| current->next->role == ROLE_PIPE))
		{
			return (SYNTAX_ERROR);
		}
		if (current->role == ROLE_REDIRECT && current->next
			&& current->next->role == ROLE_REDIRECT)
		{
			return (SYNTAX_ERROR);
		}
		if (current->role == ROLE_PIPE && current->next
			&& current->next->role == ROLE_REDIRECT)
		{
			return (SYNTAX_ERROR);
		}
		current = current->next;
	}
	return (SYNTAX_VALID);
}
