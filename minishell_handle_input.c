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

#include "minishell.h"

static char *get_next_token(char **input, t_special_char_struct *special_char, int *position)
{
    char *start = *input;
    char *token = start;
    char quote_char = '\0';

    while (*start == ' ' || *start == '\t')
    {
        start++;
        (*position)++;
    }

    if (*start == '\'' || *start == '"')
    {
        quote_char = *start;
        special_char->type = (quote_char == '\'') ? QUOTE : DOUBLE_QUOTE;
        special_char->count = 2;
        special_char->position = *position;
        start++;
        (*position)++;
        token = start;
        while (*start && *start != quote_char)
        {
            start++;
            (*position)++;
        }
        if (*start == quote_char)
        {
            *start = '\0';
            start++;
            (*position)++;
        }
    }
    else if (*start == '<' && *(start + 1) == '<')
    {
        special_char->type = REDIR_IN;
        special_char->count = 2;
        special_char->position = *position;
        token = start;
        start += 2;
        (*position) += 2;
        *start = '\0';
        start++;
        (*position)++;
    }
    else if (*start == '>' && *(start + 1) == '>')
    {
        special_char->type = REDIR_OUT;
        special_char->count = 2;
        special_char->position = *position;
        token = start;
        start += 2;
        (*position) += 2;
        *start = '\0';
        start++;
        (*position)++;
    }
    else if (is_special_char(*start))
    {
        special_char->type = (e_special_char)*start;
        special_char->count = 1;
        special_char->position = *position;
        token = start;
        start++;
        (*position)++;
        *start = '\0';
        start++;
        (*position)++;
    }
    else
    {
        while (*start && *start != ' ' && *start != '\t' && !is_special_char(*start))
        {
            start++;
            (*position)++;
        }
        if (*start)
        {
            *start = '\0';
            start++;
            (*position)++;
        }
    }

    *input = start;
    return token;
}

static int tokenize_input(t_parsed_input *parsed_input, char *input)
{
    char *token;
    int i = 0;
    int position = 0;

    while (*input && i < parsed_input->token_count)
    {
        token = get_next_token(&input, &parsed_input->special_char[i], &position);
        if (*token)
        {
            parsed_input->token[i] = strdup(token);
            if (!parsed_input->token[i])
            {
                printf("Error: Failed to allocate memory for token\n");
                return 0;
            }
            i++;
        }
    }
    parsed_input->token_count = i;
    parsed_input->token[i] = NULL;
    return 1;
}

static int count_tokens(char *input)
{
    int count = 0;
    char quote_char = 0;
    int in_token = 0;

    while (*input)
    {
        if (quote_char)
        {
            if (*input == quote_char)
            {
                quote_char = 0;
            }
        }
        else if (*input == '\'' || *input == '"')
        {
            if (!in_token)
            {
                count++;
                in_token = 1;
            }
            quote_char = *input;
        }
        else if (*input == ' ' || *input == '\t')
        {
            in_token = 0;
        }
        else if (!in_token)
        {
            count++;
            in_token = 1;
        }
        input++;
    }
    return count;
}

static int allocate_tokens(t_parsed_input *parsed_input, char *input)
{
    parsed_input->token_count = count_tokens(input);

    parsed_input->token = malloc(sizeof(char *) * (parsed_input->token_count + 1));
    parsed_input->special_char = malloc(sizeof(t_special_char_struct) * parsed_input->token_count);
    if (!parsed_input->token || !parsed_input->special_char)
    {
        printf("Error: Failed to allocate memory for tokens or special characters\n");
        return 0;
    }

    for (int i = 0; i < parsed_input->token_count; i++)
    {
        init_special_char_handling(&parsed_input->special_char[i]);
    }

    return 1;
}

int handle_input(char *input)
{
    t_parsed_input parsed_input;
    int i;

    init_parsed_input(&parsed_input);

    if (!allocate_tokens(&parsed_input, input))
    {
        printf("Error: Failed to allocate tokens\n");
        return 1;
    }

    if (!tokenize_input(&parsed_input, input))
    {
        printf("Error: Failed to tokenize input\n");
        free_parsed_input(&parsed_input);
        return 1;
    }

    for (i = 0; i < parsed_input.token_count; i++)
    {
        printf("Token %d: %s\n", i, parsed_input.token[i]);
        if (parsed_input.special_char[i].count > 0)
        {
            const char *position_desc = "";
            switch (parsed_input.special_char[i].type)
            {
                case QUOTE:
                case DOUBLE_QUOTE:
                    position_desc = "(enclosing Token)";
                    break;
                case REDIR_IN:
                case REDIR_OUT:
                    position_desc = "(before Token)";
                    break;
                default:
                    position_desc = "";
            }
            printf("special char: %c count: %d position: %s\n",
                   parsed_input.special_char[i].type,
                   parsed_input.special_char[i].count,
                   position_desc);
        }
    }

    free_parsed_input(&parsed_input);

    return 0;
}

//not used yet for parsing

/*static int process_special_chars(t_parsed_input *parsed_input)
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
}*/

//not used yet for parsing

/*static int create_commands(t_parsed_input *parsed_input)
{
    int i;
    t_command *current_cmd;

    parsed_input->commands = malloc(sizeof(t_command *) * (parsed_input->token_count + 1));
    if (!parsed_input->commands)
    {
        printf("Error: Failed to allocate memory for commands\n");
        return 0;
    }

    current_cmd = malloc(sizeof(t_command));
    if (!current_cmd)
    {
        printf("Error: Failed to allocate memory for current command\n");
        return 0;
    }
    init_command(current_cmd);
    parsed_input->commands[0] = current_cmd;

    for (i = 0; i < parsed_input->token_count; i++)
    {
        if (parsed_input->special_char[i].type == PIPE)
        {
            current_cmd->next = malloc(sizeof(t_command));
            if (!current_cmd->next)
            {
                printf("Error: Failed to allocate memory for next command\n");
                return 0;
            }
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
            if (current_cmd->arg_count >= MAX_ARGS)
            {
                printf("Error: Too many arguments\n");
                return 0;
            }
            current_cmd->args[current_cmd->arg_count] = ft_strdup(parsed_input->token[i]);
            if (!current_cmd->args[current_cmd->arg_count])
            {
                printf("Error: Failed to allocate memory for argument\n");
                return 0;
            }
            current_cmd->arg_count++;
        }
    }
    return 1;
}*/

//most probably not needeed in newer approach

/*t_parsed_input *parsing(char *input)
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
}*/

/*void process_special_char(char *token, t_special_char_struct *special_char)
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
}*/
