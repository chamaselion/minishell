/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:57:37 by bszikora          #+#    #+#             */
/*   Updated: 2024/09/25 13:36:16 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void ft_echo(char *input)
{
    char *args;
    int t_newline;

    args = input + 5;
    t_newline = 1;
    if (strncmp(args, "-n ", 3) == 0)
    {
        t_newline = 0;
        args += 3;
    }
    printf("%s", args);
    if (t_newline)
        printf("\n");
}

int main()
{
    char *input;

    input = readline("minishell> ");
    if (input == NULL)
    {
        perror("readline");
        exit(EXIT_FAILURE);
    }

    add_history(input);

    if (strncmp(input, "echo ", 5) == 0)
    {
        ft_echo(input);
    }
    else
    {
        printf("Command not recognized.\n");
    }

    free(input);
    return 0;
}