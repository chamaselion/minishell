#include "minishell.h"

void parse_input(char *input, t_shell *shell) 
{
    char **tokens = ft_split(input, ' ');
    if (!tokens) return;

    t_command *current_cmd = create_command();
    shell->commands = current_cmd;

    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            current_cmd->next = create_command();
            current_cmd = current_cmd->next;
        } else if (strcmp(tokens[i], "<") == 0 && tokens[i+1]) {
            current_cmd->input = ft_strdup(tokens[++i]);
        } else if (strcmp(tokens[i], ">") == 0 && tokens[i+1]) {
            current_cmd->output = ft_strdup(tokens[++i]);
            current_cmd->append = 0;
        } else if (strcmp(tokens[i], ">>") == 0 && tokens[i+1]) {
            current_cmd->output = ft_strdup(tokens[++i]);
            current_cmd->append = 1;
        } else {
            if (!current_cmd->command) {
                current_cmd->command = ft_strdup(tokens[i]);
            } else {
                current_cmd->args[current_cmd->arg_count++] = ft_strdup(tokens[i]);
            }
        }
    }
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

void write_parsed_input_to_file(t_shell *shell) {
    FILE *fp = fopen("parsed_input.txt", "w");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    t_command *cmd = shell->commands;
    while (cmd) {
        fprintf(fp, "Command: %s\n", cmd->command);
        fprintf(fp, "Arguments: ");
        for (int i = 0; i < cmd->arg_count; i++) {
            fprintf(fp, "%s ", cmd->args[i]);
        }
        fprintf(fp, "\n");
        if (cmd->input) {
            fprintf(fp, "Input file: %s\n", cmd->input);
        }
        if (cmd->output) {
            fprintf(fp, "Output file: %s (Append: %s)\n", cmd->output);
        }
        fprintf(fp, "\n");
        cmd = cmd->next;
    }

    fclose(fp);
}

void execute_command(t_command *cmd, t_shell *shell) {
    // This function will handle the execution of commands
    // implement the logic for built-in commands and external commands here
    // For now, we'll just print the command details
    printf("Executing command: %s\n", cmd->command);
    // Add your command execution logic here
}

int main() 
{
    t_shell shell;
    char *input;

    init_shell(&shell);
    while ((input = readline("minishell> ")) != NULL) {
        if (strlen(input) > 0) {
            add_history(input);
            parse_input(input, &shell);
            write_parsed_input_to_file(&shell);

            // Execute commands
            t_command *cmd = shell.commands;
            while (cmd) 
			{
                execute_command(cmd, &shell);
                cmd = cmd->next;
            }
            free_commands(shell.commands);
            shell.commands = NULL;
        }
        free(input);
    }
    printf("\nExiting minishell...\n");
    return 0;
}
