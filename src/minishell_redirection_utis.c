/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirection_utis.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 19:34:35 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/01 15:59:01 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect_list	*new_redirect_node(t_token *token)
{
	t_redirect_list	*node;

	node = malloc(sizeof(t_redirect_list));
	if (!node)
		return (NULL);
	node->token = token;
	node->next = NULL;
	return (node);
}

void	add_redirect(t_redirect_list **head, t_token *token)
{
	t_redirect_list	*new;
	t_redirect_list	*current;

	new = new_redirect_node(token);
	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

void	free_redirect_list(t_redirect_list *head)
{
	t_redirect_list	*current;
	t_redirect_list	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
