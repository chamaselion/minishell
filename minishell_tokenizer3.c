/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:55:23 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/23 14:06:28 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_raw_token	*create_raw_token(const char *segment, t_quote_state quote_state)
{
	t_raw_token	*token;

	token = malloc(sizeof(t_raw_token));
	if (token == NULL)
		return (NULL);
	token->segment = ft_strdup(segment);
	token->quote_state = quote_state;
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

t_raw_token	*handle_single_quote_mark(void)
{
	char		quote_str[2];
	t_raw_token	*token;

	quote_str[0] = '\'';
	quote_str[1] = '\0';
	token = create_raw_token(quote_str, NO_QUOTE);
	return (token);
}

t_raw_token	*handle_double_quote_mark(void)
{
	char		quote_str[2];
	t_raw_token	*token;

	quote_str[0] = '"';
	quote_str[1] = '\0';
	token = create_raw_token(quote_str, NO_QUOTE);
	return (token);
}

t_raw_token	*handle_non_quote_segment(const char **input, t_shell *shell)
{
	const char	*start;
	char		*segment;
	char		*temp;
	t_raw_token	*token;

	start = *input;
	while (**input && !is_whitespace(**input) && !is_quote_char(**input)
		&& !is_redirection((char*)*input) && !is_pipe((char*)*input))
		(*input)++;
	if (*input == start)
		return (NULL);
	segment = ft_strndup(start, *input - start);
	temp = resolve_variables_str(segment, shell);
	temp = purge_quotes_from_arg(temp);
	free(segment);
	if (!temp)
		return (NULL);
	token = create_raw_token(temp, NO_QUOTE);
	free(temp);
	return (token);
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
