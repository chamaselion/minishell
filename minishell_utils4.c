/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/12/09 12:55:37 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/09 18:19:29 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_whitespace(char *input)
{
	while (*input && is_whitespace(*input))
	{
		input++;
	}
	return (input);
}

char	**convert_environment_to_array(t_env_var *environment)
{
	char		**envarr;
	t_env_var	*current;
	int			idx;

	envarr = (char **)malloc(sizeof(char *) * (ft_lstsize(environment) + 1));
	if (!envarr)
		return (NULL);
	current = environment;
	idx = 0;
	while (current)
	{
		envarr[idx] = ft_strdup(current->string);
		current = current->next;
		idx++;
	}
	envarr[idx] = 0;
	return (envarr);
}

void	free_split_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	ft_lstsize(t_env_var *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	remove_last_empty_command(t_command *head_cmd, t_command *current_cmd)
{
	t_command	*prev_cmd;

	if (current_cmd->command == NULL && current_cmd->args == NULL)
	{
		prev_cmd = head_cmd;
		while (prev_cmd->next != current_cmd)
		{
			prev_cmd = prev_cmd->next;
		}
		prev_cmd->next = NULL;
		free(current_cmd);
	}
}
