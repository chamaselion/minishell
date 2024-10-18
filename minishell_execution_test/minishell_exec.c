/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:12:12 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/17 11:37:36 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mshell_exec.h"

static char **create_command_options(void)
{
	static char *res[8];

	res[0] = "echo";
	res[1] = "cd";
	res[2] = "pwd";
	res[3] = "export";
	res[4] = "unset";
	res[5] = "env";
	res[6] = "exit";
	res[7] = NULL;

	return (res);
}

void functions_init(func_ptr func_array[])
{
	func_array[0] = ft_echo;
	func_array[1] = ft_cd;
	func_array[2] = ft_pwd;
	func_array[3] = ft_export;
	func_array[4] = ft_unset;
	func_array[5] = ft_env;
	func_array[6] = ft_exit;
}

int ft_linked_list_len(t_command *cmd)
{
	int length = 0;
	t_command *current = cmd;

	while (current != NULL)
	{
		length++;
		current = current->next;
	}

	return (length);
}

int *create_queue_of_execs(t_command *cmd, char **command_options, int *exec_len)
{
	t_command *current;
	int *queue;
	int i;
	int c;
	int len;

    len = *exec_len;
	i = 0;
	*exec_len = ft_linked_list_len(cmd);
	queue = malloc(sizeof(int) * len);
	if (!queue)
		return (NULL);

	current = cmd;
	while (current)
	{
		c = 0;
		while (c <= 6)
		{
			if (strcmp(current->command, command_options[c]))
			{
				queue[i] = c;
				i++;
			}
			c++;
		}
		current = current->next;
	}
	if (i < *exec_len)
		return (NULL);
	return (queue);
}

void execute_command(t_command *cmd, t_shell *shell)
{
	func_ptr func_array[7];
	static char **command_options;
	int *exec_queue;
	int exec_len;
	int c;
	t_command *temp;

	temp = cmd;
	c = 0;
	command_options = create_command_options();
	exec_queue = create_queue_of_execs(cmd, command_options, &exec_len);
	if (!exec_queue)
		return ;
	functions_init(func_array);
	while (c < exec_len)
	{
		func_array[exec_queue[c]](temp, shell);
		c++;
		temp = temp->next;
	}
	free(exec_queue);
}

t_command *create_command_node(char *command, char *argus)
{
    t_command *new_node = (t_command *)malloc(sizeof(t_command));
    if (!new_node)
        return NULL;
    
    new_node->command = command;
    new_node->input = NULL;
    
    // Allocate memory for args array
    new_node->args = (char **)malloc(2 * sizeof(char *));
    if (!new_node->args)
    {
        free(new_node);
        return NULL;
    }
    
    // Allocate memory for the argument string and copy it
    new_node->args[0] = strdup(argus);
    if (!new_node->args[0])
    {
        free(new_node->args);
        free(new_node);
        return NULL;
    }
    
    new_node->args[1] = NULL; // Null-terminate the args array
    new_node->arg_count = 1;
    new_node->output = NULL;
    new_node->next = NULL;
    new_node->append = 0;
    
    return new_node;
}

void append_command_node(t_command **head, char *command, char *argus)
{
    t_command *new_node = create_command_node(command, argus);
    if (!new_node)
        return;
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        t_command *temp = *head;
        while (temp->next != NULL)
        {
			usleep(100);
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void	init_cmd(t_command *cmd)
{
	cmd->command = NULL;
	cmd->input = NULL;
	//cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->output = NULL;
	cmd->next = NULL;
	cmd->append = 0;
	cmd->priority;
}

int main()
{
    t_command *commands = NULL;
    t_shell shell;

	//init_cmd(commands);
	
    append_command_node(&commands, "echo", "1");
    append_command_node(&commands, "cd", "2");
    append_command_node(&commands, "pwd", "3");
    append_command_node(&commands, "export", "4");
    append_command_node(&commands, "unset", "5");
    append_command_node(&commands, "env", "6");
    append_command_node(&commands, "exit", "7");

    shell.commands = commands;

    execute_command(commands, &shell);

    t_command *temp;
    while (commands != NULL)
    {
        temp = commands;
        commands = commands->next;
        free(temp);
    }
   printf("return\n");
    return 0;
}