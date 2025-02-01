/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_concatenation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <mnaumann@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:56:10 by root              #+#    #+#             */
/*   Updated: 2025/02/01 19:43:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_concatable(t_token *current, t_token *next, int did_concat)
{
	if (current->role != ROLE_ARGUMENT)
		return (0);
	if (current->separated == 0)
		return (1);
	if (current->separated == 1)
		return (0);
	if (current->quote_state == NO_QUOTE
		&& next->quote_state == NO_QUOTE
		&& !did_concat)
		return (0);
	if (!current->separated || current->role == ROLE_VARIABLE
		|| next->role == ROLE_VARIABLE)
		return (1);
	return (0);
}

static int	try_concat(t_token *current, t_token *next, int *did_concat)
{
	char	*new_content;

	new_content = ft_strjoin(current->content, next->content);
	if (!new_content)
		return (0);
	current->content = new_content;
	current->role = ROLE_ARGUMENT;
	if ((next->quote_state != NO_QUOTE && next->next)
		|| ft_strcmp(next->content, "") == 0
		|| next->role == ROLE_VARIABLE)
	{
		current->separated = next->separated;
	}
	current->next = next->next;
	if (next->next)
		next->next->prev = current;
	free(next->content);
	free(next);
	*did_concat = 1;
	return (1);
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
			if (is_concatable(current, next, did_concat))
			{
				if (!try_concat(current, next, &did_concat))
					return ;
			}
			else
				current = current->next;
		}
	}
}

/*void concatenate_tokens(t_token **token_list)
{
	t_token *current;
	t_token *next;
	char    *new_content;
	int     did_concat;

	if (!token_list || !*token_list)
		return;
	do
	{
		did_concat = 0;
		current = *token_list;
		while (current && current->next)
		{
			next = current->next;
			if (current->role == ROLE_ARGUMENT && 
				!is_quote_char(*current->content) &&
				!(current->quote_state == NO_QUOTE &&
				next->quote_state == NO_QUOTE && !did_concat) &&
				!is_quote_char(*next->content) &&
				(!current->separated || 
				current->role == ROLE_VARIABLE || 
				next->role == ROLE_VARIABLE))
			{
				new_content = ft_strjoin(current->content, next->content);
				if (!new_content)
					return;
				current->content = new_content;
				current->role = ROLE_ARGUMENT;
				if ((next->quote_state != NO_QUOTE && next->next) 
					|| ft_strcmp(next->content, "") == 0
					|| next->role == ROLE_VARIABLE)
					current->separated = next->separated;
				current->next = next->next;
				if (next->next)
					next->next->prev = current;
				free(next->content);
				free(next);
				did_concat = 1;
			}
			else
				current = current->next;
		}
	} while (did_concat);
}*/
