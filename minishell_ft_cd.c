/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:10:19 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/31 22:00:14 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd_arg_checker(t_command *cmd)
{
	char	*path;

	if (cmd->arg_count > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	path = cmd->args[0];
	if (path)
	{
		if (access(path, F_OK) != 0)
			return (ft_putstr_fd("cd: no such file or directory\n",
					STDERR_FILENO), 1);
		if (access(path, X_OK) != 0)
			return (ft_putstr_fd("cd: permission denied\n", STDERR_FILENO), 1);
	}
	return (0);
}

int	ft_cd(t_command *cmd)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*path;

	if (ft_cd_arg_checker(cmd) == 1)
		return (1);
	path = cmd->args[0];
	if (!path)
		return (ft_putstr_fd("cd: path is required\n", STDERR_FILENO), 1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (ft_putstr_fd("cd: getcwd failed\n", STDERR_FILENO), 1);
	if (chdir(path) != 0)
		return (ft_putstr_fd("cd\n", STDERR_FILENO), free(old_pwd), 1);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (ft_putstr_fd("cd: getcwd failed\n", STDERR_FILENO), free(old_pwd),
			1);
	set_or_create_env_var(&cmd->shell->env_vars, "OLDPWD", old_pwd);
	set_or_create_env_var(&cmd->shell->env_vars, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
