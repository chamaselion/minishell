/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_concatenation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <mnaumann@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:56:10 by root              #+#    #+#             */
/*   Updated: 2025/02/01 20:58:14 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_concatable(t_token *current, t_token *next)
{
	if (current->role != ROLE_ARGUMENT)
		return (0);
	if (current->separated == 0)
		return (1);
	if (current->separated == 1)
		return (0);
	if (!current->separated || current->role == ROLE_VARIABLE
		|| next->role == ROLE_VARIABLE)
		return (1);
	return (0);
}

static int	try_concat(t_token *current, t_token *next, int did_concat)
{
	char	*new_content;

	new_content = ft_strjoin(current->content, next->content);
	if (!new_content)
		return (0);
	current->content = new_content;
	current->role = ROLE_ARGUMENT;
	current->separated = next->separated;
	current->next = next->next;
	if (next->next)
		next->next->prev = current;
	free(next->content);
	free(next);
	did_concat = 1;
	return (did_concat);
}

void	concatenate_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*next;
	int		did_concat;

	if (!token_list || !*token_list)
		return ;
	did_concat = 1;
	while (did_concat)
	{
		did_concat = 0;
		current = *token_list;
		while (current && current->next)
		{
			next = current->next;
			if (is_concatable(current, next))
			{
				if (!try_concat(current, next, did_concat))
					return ;
			}
			else
				current = current->next;
		}
	}
}
