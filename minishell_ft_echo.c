/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_echo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:44:25 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/21 15:04:38 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int ft_echo(t_command *cmd)
{
    int t_newline;
    int i;

	t_newline = 1;
	i = 0;
	if (!cmd)
		return (1);
    if (cmd->args[0] && strcmp(cmd->args[0], "-n") == 0)
    {
        t_newline = 0;
        i = 1;
    }
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
	
    if (t_newline)
	{
    	printf("\n");
	}
	return (0);
}
