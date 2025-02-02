/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:08:59 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:08:59 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*convert_raw_token(t_raw_token *raw_token)
{
	t_token	*new_token;

	if (raw_token == NULL)
		return (NULL);
	if (raw_token->segment == NULL || *raw_token->segment == '\0')
		return (NULL);
	new_token = malloc(sizeof(t_token));
	init_token(new_token);
	if (new_token == NULL)
		return (NULL);
	new_token->content = ft_strdup(raw_token->segment);
	new_token->quote_state = raw_token->quote_state;
	if (new_token->quote_state == WITHIN_SINGLE_QUOTE
		|| new_token->quote_state == WITHIN_DOUBLE_QUOTE)
		new_token->role = ROLE_ARGUMENT;
	new_token->command_expected = 0;
	new_token->syntax_state = 0;
	new_token->separated = raw_token->separated;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

int	is_raw_token_list_empty(t_raw_token *raw_token_head)
{
	if (raw_token_head == NULL || *raw_token_head->segment == '\0')
	{
		free_raw_tokens(raw_token_head);
		return (1);
	}
	return (0);
}

void	link_token_to_list(t_token **new_head, t_token **current_new,
		t_token *new_token)
{
	if (!(*new_head))
		*new_head = new_token;
	else
	{
		(*current_new)->next = new_token;
		new_token->prev = *current_new;
	}
	*current_new = new_token;
}

t_token	*convert_raw_token_list(t_raw_token *raw_token_head)
{
	t_token		*new_head;
	t_token		*current_new;
	t_raw_token	*current_raw;
	t_token		*new_token;

	new_head = NULL;
	current_new = NULL;
	current_raw = raw_token_head;
	if (is_raw_token_list_empty(raw_token_head))
		return (NULL);
	while (current_raw != NULL && current_raw->segment != NULL)
	{
		new_token = convert_raw_token(current_raw);
		if (new_token != NULL)
			link_token_to_list(&new_head, &current_new, new_token);
		if (current_raw->next)
			current_raw = current_raw->next;
		else
			break ;
	}
	if (finalizing_token_list(new_head) == NULL)
		return (free_tokens(new_head), free_raw_tokens(raw_token_head), NULL);
	free_raw_tokens(raw_token_head);
	return (new_head);
}
