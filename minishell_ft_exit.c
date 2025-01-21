/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:09:29 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/21 19:54:48 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int ft_exit(t_command *cmd)
{
    int exit_status;
    int i;
    
    ft_putstr_fd("exit\n", STDERR_FILENO);
    
    if (!cmd->args[0])
    {
        exit_status = cmd->shell->last_exit_code;
        exit(exit_status);
    }
    i = 0;
    while (cmd->args[0][i])
    {
        if (i == 0 && (cmd->args[0][i] == '-' || cmd->args[0][i] == '+'))
            i++;
        else if (!ft_isdigit(cmd->args[0][i]))
        {
            ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            exit(2);
        }
        i++;
    }
    
    if (cmd->args[1])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    
    exit_status = ft_atoi(cmd->args[0]) & 255;
    exit(exit_status);
}
