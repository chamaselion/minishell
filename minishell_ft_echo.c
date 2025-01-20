/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_echo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:44:25 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/15 15:28:15 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void ft_echo(t_command *cmd)
{
    int t_newline = 1;
    int i = 0; // Start from the first argument

	printf("ftyay:");
    if (cmd->args[0] && strcmp(cmd->args[0], "-n") == 0)
    {
        t_newline = 0;
        i = 1; // Skip the "-n" argument
    }

    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }

    if (t_newline)
        printf("\n");
}
