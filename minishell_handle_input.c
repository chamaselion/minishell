/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_handle_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:19:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/17 20:55:21 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_parsed_input	*parsing(char *input)
{
	t_parsed_input	*parsed_input;

	init_parsed_input(parsed_input);
	parsed_input->token = ft_strtok(input, parsed_input->delimiters); //add token count to function, see struct
	if (parsed_input->token == NULL)
		return(free(parsed_input), NULL);
	return (parsed_input);




	


}

int handle_input(char *input)	//Function to handle the input by the user, should lead to a much
								//more complex function laterhandling most of the stuff.
{
	t_parsed_input	*parsed_input;

	parsed_input = parsing(input); 

	if (input == NULL)
		return 1;
	printf("User input: %s\n", input);
	free(input);
	return 0;
}
