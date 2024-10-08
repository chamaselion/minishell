#include "minishell.h"

void free_parsed_input(t_parsed_input *parsed_input)
{
    for (int i = 0; i < parsed_input->token_count; i++)
    {
        free(parsed_input->token[i]);
    }
    free(parsed_input->token);
    free(parsed_input->special_char);
}

void free_command(t_command *cmd) 
{
	int i;

	i = 0;
    free(cmd->command);
	while (cmd->args && cmd->args[i]) 
	{
		free(cmd->args[i]);
		i++;
	}
    free(cmd->input);
    free(cmd->output);
    free(cmd);
}

void free_commands(t_command *cmd) 
{
    while (cmd) 
	{
        t_command *next = cmd->next;
        free_command(cmd);
        cmd = next;
    }
}