/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:10:19 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/20 19:16:18 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void ft_cd(t_command *cmd)
{
    char *old_pwd;
    char *new_pwd;
    char *path;

    path = cmd->args[0];
    if (!path)
    {
        ft_putstr_fd("cd: path is required\n", STDERR_FILENO);
        return;
    }
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        perror("cd: getcwd failed");
        return;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        free(old_pwd);
        return;
    }
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("cd: getcwd failed");
        free(old_pwd);
        return;
    }
    set_or_create_env_var(&cmd->shell->env_vars, "OLDPWD", old_pwd);
    set_or_create_env_var(&cmd->shell->env_vars, "PWD", new_pwd);
    free(old_pwd);
    free(new_pwd);
}
