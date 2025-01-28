/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_quote_handling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:40:32 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/28 17:08:46 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_raw_token	*handle_single_quote_segment(const char **input, int *pos)
{
	const char	*start;
	char		*content;
	t_raw_token	*token;

	(*input)++;
	start = *input;
	while (**input != '\0' && **input != '\'')
		(*input)++;
	content = ft_strndup(start, *input - start);
	token = create_raw_token(content, WITHIN_SINGLE_QUOTE, *pos);
	free(content);
	return (token);
}

void	single_quote_wrapper(const char **input, int *pos, t_raw_token **first,
		t_raw_token **last)
{
	t_raw_token	*token;

	token = handle_single_quote_mark(pos);
	append_raw_token(first, last, token);
	token = handle_single_quote_segment(input, pos);
	append_raw_token(first, last, token);
	if (**input == '\'')
	{
		token = handle_single_quote_mark(pos);
		append_raw_token(first, last, token);
		(*input)++;
	}
}

void	double_quote_wrapper(const char **input, int *pos, t_raw_token **first,
		t_raw_token **last, t_env_var *env)
{
	t_raw_token	*token;

	token = handle_double_quote_mark(pos);
	append_raw_token(first, last, token);
	token = handle_double_quote_segment(input, pos, env);
	append_raw_token(first, last, token);
	if (**input == '"')
	{
		token = handle_double_quote_mark(pos);
		append_raw_token(first, last, token);
		(*input)++;
	}
}

t_raw_token	*handle_double_quote_segment(const char **input,
				int *pos, t_env_var *env)
{
	const char	*start;
	char		*content;
	char		*expanded_content;
	t_raw_token	*token;

	(*input)++;
	start = *input;
	while (**input != '\0' && **input != '"')
		(*input)++;
	content = ft_strndup(start, *input - start);
	expanded_content = expand_double_quote_content(content, env);
	token = create_raw_token(expanded_content, WITHIN_DOUBLE_QUOTE, *pos);
	free(content);
	free(expanded_content);
	return (token);
}

void	update_quote_state(const char *p, int *quote_state)
{
	if (*p == '\'' && *quote_state == NO_QUOTE)
		*quote_state = WITHIN_SINGLE_QUOTE;
	else if (*p == '"' && *quote_state == NO_QUOTE)
		*quote_state = WITHIN_DOUBLE_QUOTE;
	else if ((*p == '\'' && *quote_state == WITHIN_SINGLE_QUOTE)
		|| (*p == '"' && *quote_state == WITHIN_DOUBLE_QUOTE))
		*quote_state = NO_QUOTE;
}
