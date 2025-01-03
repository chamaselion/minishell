/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free_and_exit2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:16:20 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/02 16:57:04 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_raw_tokens(t_raw_token *first_token)
{
	t_raw_token	*temp;

	while (first_token != NULL)
	{
		temp = first_token;
		first_token = first_token->next;
		free(temp->segment);
		free(temp);
	}
}
