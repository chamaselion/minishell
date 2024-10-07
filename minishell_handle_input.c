/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_handle_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:19:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/19 11:30:26 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int count_and_allocate_tokens(t_parsed_input *parsed_input, char *input)
{
    char *token;
    const char *delim = parsed_input->delimiters;

    parsed_input->token_count = 0;
    token = ft_strtok(input, delim);
    while (token != NULL)
    {
        parsed_input->token_count++;
        token = ft_strtok(NULL, delim);
    }

    parsed_input->token = malloc(sizeof(char *) * (parsed_input->token_count + 1));
    return (parsed_input->token != NULL);
}

static int tokenize_input(t_parsed_input *parsed_input, char *input)
{
    char *token;
    int i = 0;
    const char *delim = parsed_input->delimiters;

    token = ft_strtok(input, delim);
    while (token != NULL)
    {
        parsed_input->token[i] = ft_strdup(token);
        if (!parsed_input->token[i])
            return 0;
        i++;
        token = ft_strtok(NULL, delim);
    }
    parsed_input->token[i] = NULL;
    return 1;
}

void process_special_char(char *token, t_special_char_struct *special_char)
{
    int count;

    if ((count = is_special_char(token, PIPE)) > 0)
    {
        special_char->type = PIPE;
        special_char->status = count;
    }
    else if ((count = is_special_char(token, REDIR_IN)) > 0)
    {
        special_char->type = REDIR_IN;
        special_char->status = count;
    }
    else if ((count = is_special_char(token, REDIR_OUT)) > 0)
    {
        special_char->type = REDIR_OUT;
        special_char->status = count;
    }
    else if ((count = is_special_char(token, DOLLAR)) > 0)
    {
        special_char->type = DOLLAR;
        special_char->status = count;
    }
    else if ((count = is_special_char(token, QUOTE)) > 0)
    {
        special_char->type = QUOTE;
        special_char->status = count;
    }
    else if ((count = is_special_char(token, DOUBLE_QUOTE)) > 0)
    {
        special_char->type = DOUBLE_QUOTE;
        special_char->status = count;
    }
    else
    {
        special_char->type = END_OF_FILE;
        special_char->status = -1;
    }
}

static int process_special_chars(t_parsed_input *parsed_input)
{
    int i;

    parsed_input->special_char = malloc(sizeof(t_special_char_struct) * parsed_input->token_count);
    if (!parsed_input->special_char)
        return 0;

    for (i = 0; i < parsed_input->token_count; i++)
    {
        init_special_char_handling(&parsed_input->special_char[i]);
        process_special_char(parsed_input->token[i], &parsed_input->special_char[i]);
    }
    return 1;
}

static int create_commands(t_parsed_input *parsed_input)
{
    int i;
    t_command *current_cmd;

    parsed_input->commands = malloc(sizeof(t_command *) * (parsed_input->token_count + 1));
    if (!parsed_input->commands)
        return 0;

    current_cmd = malloc(sizeof(t_command));
    if (!current_cmd)
        return 0;
    init_command(current_cmd);
    parsed_input->commands[0] = current_cmd;

    for (i = 0; i < parsed_input->token_count; i++)
    {
        if (parsed_input->special_char[i].type == PIPE)
        {
            current_cmd->next = malloc(sizeof(t_command));
            if (!current_cmd->next)
                return 0;
            current_cmd = current_cmd->next;
            init_command(current_cmd);
            parsed_input->commands[i + 1] = current_cmd;
        }
        else if (parsed_input->special_char[i].type == REDIR_IN || parsed_input->special_char[i].type == REDIR_OUT)
        {
            if (parsed_input->special_char[i].type == REDIR_IN)
                current_cmd->input = ft_strdup(parsed_input->token[i + 1]);
            else
                current_cmd->output = ft_strdup(parsed_input->token[i + 1]);
            i++; // Skip the next token as it is the file name
        }
        else
        {
            current_cmd->args[current_cmd->arg_count] = ft_strdup(parsed_input->token[i]);
            current_cmd->arg_count++;
        }
    }
    return 1;
}

t_parsed_input *parsing(char *input)
{
    t_parsed_input *parsed_input;

    parsed_input = malloc(sizeof(t_parsed_input));
    if (!parsed_input)
        return NULL;
    init_parsed_input(parsed_input);

    if (!count_and_allocate_tokens(parsed_input, input))
    {
        free_parsed_input(parsed_input);
        return NULL;
    }

    if (!tokenize_input(parsed_input, input))
    {
        free_parsed_input(parsed_input);
        return NULL;
    }

    if (!process_special_chars(parsed_input))
    {
        free_parsed_input(parsed_input);
        return NULL;
    }

    if (!create_commands(parsed_input))
    {
        free_parsed_input(parsed_input);
        return NULL;
    }

    return parsed_input;
}

int handle_input(char *input)
{
    t_parsed_input *parsed_input;
    int i;

    if (input == NULL)
        return 1;

    parsed_input = parsing(input);
    if (!parsed_input)
    {
        printf("Error: Failed to parse input\n");
        free(input);
        return 1;
    }

    // Print parsed tokens (for debugging)
    for (i = 0; i < parsed_input->token_count; i++)
    {
        printf("Token %d: %s\n", i, parsed_input->token[i]);
    }

    // TODO: Process the parsed input (execute commands, etc.)

    free_parsed_input(parsed_input);
    free(input);
    return 0;
}