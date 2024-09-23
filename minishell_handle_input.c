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
    return parsed_input;
}

static int count_and_allocate_tokens(t_parsed_input *parsed_input, char *input)
{
    char *token;
    
    parsed_input->token_count = 0;
    token = ft_strtok(input, parsed_input->delimiters);
    while (token != NULL)
    {
        parsed_input->token_count++;
        token = ft_strtok(NULL, parsed_input->delimiters);
    }

    parsed_input->token = malloc(sizeof(char *) * (parsed_input->token_count + 1));
    return (parsed_input->token != NULL);
}

static int tokenize_input(t_parsed_input *parsed_input, char *input)
{
    char *token;
    int i = 0;

    token = ft_strtok(input, parsed_input->delimiters);
    while (token != NULL)
    {
        parsed_input->token[i] = ft_strdup(token);
        if (!parsed_input->token[i])
            return 0;
        i++;
        token = ft_strtok(NULL, parsed_input->delimiters);
    }
    parsed_input->token[i] = NULL;
    return 1;
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


int handle_input(char *input)
{
    t_parsed_input *parsed_input;

    if (input == NULL)
        return 1;
    parsed_input = parsing(input);
    if (!parsed_input)
    {
        printf("Error: Failed to parse input\n");
        free(input);
        return 1;
    }

    // TODO: Process the parsed input (execute commands, etc.)

    free_parsed_input(parsed_input);
    free(input);
    return 0;
}
/*int handle_input(char *input)	//Function to handle the input by the user, should lead to a much
								//more complex function laterhandling most of the stuff.
{
	t_parsed_input	*parsed_input;

	parsed_input = parsing(input); 

	if (input == NULL)
		return 1;
	printf("User input: %s\n", input);
	free(input);
	return 0;
}*/
