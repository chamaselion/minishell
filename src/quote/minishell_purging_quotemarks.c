/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_purging_quotemarks.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:07:59 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:07:59 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_unclosed(t_raw_token *list, t_shell *shell)
{
	t_raw_token		*current;
	int				quote_state;

	if (list == NULL)
		return (0);
	current = list;
	quote_state = current->quote_state;
	while (current != NULL)
	{
		quote_state = current->quote_state;
		current = current->next;
	}
	if (quote_state != NO_QUOTE)
	{
		ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO);
		return (update_exit_code(shell, 2), 1);
	}
	return (0);
}

void	remove_token(t_token **tokens, t_token *to_remove)
{
	t_token	*prev;
	t_token	*current;

	prev = NULL;
	current = *tokens;
	while (current)
	{
		if (current == to_remove)
		{
			if (prev)
				prev->next = current->next;
			else
				*tokens = current->next;
			free(current->content);
			free(current);
			current = NULL;
			break ;
		}
		prev = current;
		current = current->next;
	}
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
	if (!purged_args)
		return (NULL);
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
	if (!purged_arg)
		return (NULL);
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
	free(arg);
	return (purged_arg);
}
