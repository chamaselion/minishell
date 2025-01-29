/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_quote_handling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:40:32 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/29 14:22:04 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_raw_token	*handle_single_quote_segment(const char **input)
{
	const char	*start;
	char		*content;
	t_raw_token	*token;

	(*input)++;
	start = *input;
	while (**input != '\0' && **input != '\'')
		(*input)++;
	content = ft_strndup(start, *input - start);
	token = create_raw_token(content, WITHIN_SINGLE_QUOTE);
	free(content);
	return (token);
}

void	single_quote_wrapper(const char **input, t_raw_list *raw_list)
{
	t_raw_token	*token;

	token = handle_single_quote_mark();
	append_raw_token(&raw_list->first, &raw_list->last, token);
	token = handle_single_quote_segment(input);
	append_raw_token(&raw_list->first, &raw_list->last, token);
	if (**input == '\'')
	{
		token = handle_single_quote_mark();
		append_raw_token(&raw_list->first, &raw_list->last, token);
		(*input)++;
	}
}

void	double_quote_wrapper(const char **input, t_raw_list *raw_list,
			t_env_var *env, t_shell *shell)
{
	t_raw_token	*token;

	token = handle_double_quote_mark();
	append_raw_token(&raw_list->first, &raw_list->last, token);
	token = handle_double_quote_segment(input, env, shell);
	append_raw_token(&raw_list->first, &raw_list->last, token);
	if (**input == '"')
	{
		token = handle_double_quote_mark();
		append_raw_token(&raw_list->first, &raw_list->last, token);
		(*input)++;
	}
}

t_raw_token	*handle_double_quote_segment(const char **input, t_env_var *env,
				t_shell *shell)
{
	const char	*start;
	char		*content;
	char		*expanded_content;
	char		*temp;
	t_raw_token	*token;

	(*input)++;
	start = *input;
	while (**input != '\0' && **input != '"')
		(*input)++;
	content = ft_strndup(start, *input - start);
	expanded_content = expand_double_quote_content(content, env);
	temp = resolve_variables_str(expanded_content, shell);
	token = create_raw_token(temp, WITHIN_DOUBLE_QUOTE);
	free(content);
	free(expanded_content);
	free(temp);
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
	else if (*p == '"' && *quote_state == WITHIN_SINGLE_QUOTE)
		*quote_state = WITHIN_SINGLE_QUOTE;
}
