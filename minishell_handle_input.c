/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_handle_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:19:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/16 18:07:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_parsed_input	*parsing(char *input)
{
	t_parsed_input	*parsed_input;

	init_parsed_input(parsed_input);
	parsed_input->input = ft_strtok(input, parsed_input->delimiters); //add token count to function, see struct
	if (parsed_input->input == NULL)
	{
		free(parsed_input);
		return NULL;
	}



	


}

int handle_input(char *input)	//Function to handle the input by the user, should lead to a much
								//more complex function laterhandling most of the stuff.
{
	t_parsed_input	*parsed_input;

	parsed_input = parsing(input) 

	if (input == NULL)
		return 1;
	printf("User input: %s\n", input);
	free(input);
	return 0;
}