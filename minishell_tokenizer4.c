/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenizer4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:36:44 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/11 13:38:27 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *finalizing_token_list(t_token *token_list)
{
    printf("Entered finalizing_token_list\n");
    if (token_list == NULL) {
        printf("Token list is NULL\n");
        return NULL;
    }

    assign_token_role(token_list);
    check_for_unclosed(token_list);
    pop_quotemark_tokens(&token_list);
    printf("Exiting finalizing_token_list\n");
    return token_list;
}

t_raw_token	*handle_input(char *input)
{
	t_raw_token	*first;
	t_raw_token	*last;
	t_raw_token	*token;
	int			pos;

	first = NULL;
	last = NULL;
	pos = 0;
	while (*input != '\0')
	{
		input = skip_whitespace(input);
		pos++;
		if (*input == '\'')
			single_quote_wrapper((const char **)&input, &pos, &first, &last);
		else if (*input == '"')
			double_quote_wrapper((const char **)&input, &pos, &first, &last);
		else
		{
			token = handle_non_quote_segment((const char **)&input, &pos);
			append_raw_token(&first, &last, token);
		}
	}
	return (first);
}
