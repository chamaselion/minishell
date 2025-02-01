/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:09:29 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/29 15:07:14 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status(t_command *cmd)
{
	int	exit_status;

	exit_status = cmd->shell->last_exit_code;
	exit(exit_status);
}

static int	get_sign_and_skip(const char *arg, int *i)
{
    int	sign = 1;
    while (arg[*i] && (arg[*i] == ' ' || arg[*i] == '\t' || arg[*i] == '\n' ||
           arg[*i] == '\r' || arg[*i] == '\v' || arg[*i] == '\f'))
        (*i)++;
    if (arg[*i] == '-' || arg[*i] == '+')
    {
        if (arg[*i] == '-')
            sign = -1;
        (*i)++;
    }
    return (sign);
}

static void	validate_digits(const char *arg, int i, int sign)
{
	long long	val;
	int			digit;

	val = 0;
	while (arg[i] && ft_isdigit(arg[i]))
	{
		digit = arg[i] - '0';
		if ((sign == 1 && val > (LLONG_MAX - digit) / 10) ||
			(sign == -1 && -val < (LLONG_MIN + digit) / 10))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd((char *)arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(2);
		}
		val = val * 10 + digit;
		i++;
	}
	if (arg[i] != '\0')
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
}

int	validate_exit_arguments(t_command *cmd)
{
	char	*arg;
	int		i;
	int		sign;

	arg = cmd->args[0];
	i = 0;
	sign = get_sign_and_skip(arg, &i);
	if (!arg[i] || !ft_isdigit(arg[i]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	validate_digits(arg, i, sign);
	return (0);
}

int	ft_exit(t_command *cmd)
{
	int	exit_status;

	if (!cmd->args[0])
		handle_exit_status(cmd);
	if (validate_exit_arguments(cmd))
		return (1);
	exit_status = ft_atoi(cmd->args[0]) & 255;
	exit(exit_status);
}
