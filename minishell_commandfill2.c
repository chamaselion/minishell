/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commandfill2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:30:29 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 13:05:26 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void remove_last_empty_command(t_command *head_cmd, t_command *current_cmd)
{
	t_command *prev_cmd;

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

int process_tokens(t_token *ct, t_command *current_cmd)
{
    while (ct && (ct->role == 3 || ct->role == 0))
	{
        if (ct->role == 3)
		{
            if (allocate_args(current_cmd, ct) == -1)
                return (-1);
        }
        ct = ct->next;
    }
    current_cmd->args = realloc(current_cmd->args, sizeof(char*) * (current_cmd->arg_count + 1));
    if (current_cmd->args == NULL)
        return (-1);
    current_cmd->args[current_cmd->arg_count] = NULL;
    return (0);
}

int fill_command_from_tokens(t_token *tokens, t_command **cmd)
{
    t_token *ct = tokens;
    t_command *current_cmd;
	
	current_cmd = create_command_list(tokens);
    if (!current_cmd)
        return (-1);
    *cmd = current_cmd;
    while (ct)
    {
        if (ct->role == 1 || ct->role == 2)
        {
            current_cmd->command = ct->content;
            if (process_tokens(ct->next, current_cmd) == -1)
                return (-1);
            if (current_cmd->next)
                current_cmd = current_cmd->next;
        }
        ct = ct->next;
    }
    return (expected_command_counter(tokens));
}

void handle_builtin(t_token *ct, t_command *current_cmd)
{
	(void)ct;
	(void)current_cmd;
	
}

void handle_redirect(t_token *ct, t_command *current_cmd)
{
    if (strcmp(ct->content, ">") == 0)
        current_cmd->output_redirection = ct->next;
    else if (strcmp(ct->content, "<") == 0)
        current_cmd->input_redirection = ct->next;
    else if (strcmp(ct->content, ">>") == 0)
        current_cmd->append_redirection = ct->next;
    else if (strcmp(ct->content, "<<") == 0)
        current_cmd->heredoc_redirection = ct->next;
    current_cmd->relation_type = 5;
}

void handle_pipe(t_command *current_cmd)
{
    if (current_cmd->next)
    {
        current_cmd->relation_type = 6; // Pipe
        current_cmd->related_to = current_cmd->next;
        current_cmd = current_cmd->next;
    }
}

void link_commands_and_tokens(t_token *tokens, t_command *cmd)
{
    t_token *ct = tokens;
    t_command *current_cmd = cmd;
	
	ct = tokens;
    current_cmd = cmd;
    while (ct)
    {
        if (ct->role == 4)
            handle_builtin(ct, current_cmd);
        else if (ct->role == 5)
        {
            handle_redirect(ct, current_cmd);
            ct = ct->next;
        }
        else if (ct->role == 6)
            handle_pipe(current_cmd);
        ct = ct->next;
    }
}
