/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:55:37 by mnaumann          #+#    #+#             */
/*   Updated: 2024/12/09 18:19:29 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *skip_whitespace(char *input) 
{
	while (*input && is_whitespace(*input)) 
	{
		input++;
	}
	return input;
}

char* expand_env_variable(const char *var_name) 
{
    if (!var_name) 
        return NULL;
    return (getenv(var_name));
}
