/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_quote_handling2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:00:57 by root              #+#    #+#             */
/*   Updated: 2025/02/02 16:46:28 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(segment);
	if (!temp || *temp == '\0')
		return (free(temp), NULL);
	if (temp)
	{
		token = create_raw_token(temp, NO_QUOTE);
		separation_check(input, token);
		free(temp);
	}
	return (token);
}

void	separation_check(const char **input, t_raw_token *token)
{
	if (**input && *(*input + 1))
	{
		if (is_whitespace(**input))
			token->separated = 1;
		else if (is_redirection((char *) *input) || is_pipe((char *) *input))
			token->separated = 1;
		else if (*(*input + 2) && is_quote_char(**input)
			&& is_quote_char(*(*input + 1)) && is_whitespace(*(*input + 2)))
			token->separated = 1;
		else if (*(*input + 2) && *(*input + 3) && is_quote_char(**input + 1)
			&& is_quote_char(**input + 2) && is_whitespace(*(*input + 1))
			&& token->quote_state != 0)
			token->separated = 1;
	}
}
