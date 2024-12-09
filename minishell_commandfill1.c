/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commandfill1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:30:14 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 12:31:38 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_command* create_new_command()
{
    t_command *new_cmd;
	
	new_cmd = (t_command*)malloc(sizeof(t_command));
    if (new_cmd == NULL)
        return (NULL);
    new_cmd->command = NULL;
    new_cmd->input = NULL;
    new_cmd->args = NULL;
    new_cmd->arg_count = 0;
    new_cmd->output = NULL;
    new_cmd->related_to = NULL;
    new_cmd->relation_type = 0;
    new_cmd->next = NULL;
    new_cmd->is_internal = 0;
    new_cmd->input_redirection = NULL;
    new_cmd->output_redirection = NULL;
    new_cmd->append_redirection = NULL;
    new_cmd->heredoc_redirection = NULL;
    return (new_cmd);
}

int	expected_command_counter(t_token *token)
{
	t_token *current_token;
	int c;

	current_token = token;
	c = 0;

	while (current_token)
	{
		if (current_token->role == 1 || current_token->role == 2)
			c++;
		current_token = current_token->next;
	}
	return (c);
}

t_command* create_command_list(t_token *tokens)
{
    int command_count;
    int c;
    t_command *current_command;
	t_command *head;

    command_count = expected_command_counter(tokens);
    head = create_new_command();
    if (head == NULL)
        return (NULL);
    current_command = head;
    c = 1;
    while (c < command_count)
	{
        current_command->next = create_new_command();
        if (current_command->next == NULL)
            return (NULL);
        current_command = current_command->next;
        c++;
    }
	return (head);
}

int allocate_args(t_command *current_cmd, t_token *ct)
{
    current_cmd->args = realloc(current_cmd->args, sizeof(char*) * (current_cmd->arg_count + 1));
    if (current_cmd->args == NULL)
	{
        return (-1);
    }
    current_cmd->args[current_cmd->arg_count] = ct->content;
    current_cmd->arg_count++;
    return (0);
}