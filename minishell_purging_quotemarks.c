/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_purging_quotemarks.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 09:35:22 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/24 17:47:32 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_unclosed(t_token *token_list)
{
    t_token	*current;
    int		quote_state;

    quote_state = NO_QUOTE;
    current = token_list;
    while (current != NULL)
    {
        const char *p = current->content;
        while (*p)
        {
            if (*p == '\'' && quote_state == NO_QUOTE)
                quote_state = WITHIN_SINGLE_QUOTE;
            else if (*p == '"' && quote_state == NO_QUOTE)
                quote_state = WITHIN_DOUBLE_QUOTE;
            else if (*p == '\'' && quote_state == WITHIN_SINGLE_QUOTE)
                quote_state = NO_QUOTE;
            else if (*p == '"' && quote_state == WITHIN_DOUBLE_QUOTE)
                quote_state = NO_QUOTE;
            p++;
        }
        current = current->next;
    }
    if (quote_state != NO_QUOTE)
    {
        printf("Unclosed quote\n");
        exit(1);
    }
}

void	remove_token(t_token **head, t_token *token)
{
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	if (*head == token)
		*head = token->next;
	free(token->content);
	free(token);
}

t_token	*pop_quotemark_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*next;

	current = *token_list;
	while (current)
	{
		next = current->next;
		if (is_quote_char(*current->content)
			&& current->quote_state == NO_QUOTE)
		{
			if (current == *token_list)
				*token_list = next;
			remove_token(token_list, current);
		}
		if (current->next == NULL)
			break ;
		current = next;
	}
	return (*token_list);
}

char	**purge_quotes_from_args(t_command *cmd)
{
	char	**purged_args;
	char	**args;
	int		i;

	i = 0;
	args = cmd->args;
	purged_args = (char **)malloc(sizeof(char *) * (cmd->arg_count + 1));
	while (*args)
	{
		purged_args[i] = purge_quotes_from_arg(*args);
		i++;
		args++;
	}
	purged_args[i] = NULL;
	free_split_array(cmd->args);
	return (purged_args);
}

char	*purge_quotes_from_arg(char *arg)
{
	char	*purged_arg;
	char	*current;
	char	*write_ptr;

	purged_arg = (char *)malloc(sizeof(char) * (ft_strlen(arg) + 1));
	current = arg;
	write_ptr = purged_arg;
	while (*current)
	{
		if (!is_quote_char(*current))
		{
			*write_ptr++ = *current;
		}
		current++;
	}
	*write_ptr = '\0';
	return (purged_arg);
}
