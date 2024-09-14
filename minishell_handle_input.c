/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_input_handles.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:19:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/10 13:20:59 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int handle_input(char *input)	//Function to handle the input by the user, should lead to a much
								//more complex function laterhandling most of the stuff.
{
	if (input == NULL)
		return 1;
	printf("User input: %s\n", input);
	free(input);
	return 0;
}