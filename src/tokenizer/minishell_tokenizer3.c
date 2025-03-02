/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:09:07 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:09:07 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_raw_token	*create_raw_token(const char *segment, t_quote_state quote_state)
{
	t_raw_token	*token;

	token = malloc(sizeof(t_raw_token));
	if (token == NULL)
		return (NULL);
	token->segment = ft_strdup(segment);
	if (token->segment == NULL)
	{
		free(token);
		return (NULL);
	}
	token->quote_state = quote_state;
	token->separated = 0;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	append_raw_token(t_raw_token **first, t_raw_token **last,
		t_raw_token *new_token)
{
	if (new_token == NULL)
		return ;
	if (*first == NULL)
	{
		*first = new_token;
		*last = new_token;
	}
	else
	{
		(*last)->next = new_token;
		new_token->prev = *last;
		*last = new_token;
	}
}

t_raw_token	*tokenize_pipe_redirection(const char **input)
{
	char		*segment;
	t_raw_token	*token;

	if ((*input)[1] == '>' || (*input)[1] == '<')
	{
		segment = ft_strndup(*input, 2);
		(*input) += 2;
	}
	else
	{
		segment = ft_strndup(*input, 1);
		(*input)++;
	}
	token = create_raw_token(segment, NO_QUOTE);
	free(segment);
	return (token);
}
