/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:09:29 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:05:56 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_sign_and_skip(const char *arg, int *i)
{
	int	sign;

	sign = 1;
	while (arg[*i] && (arg[*i] == ' ' || arg[*i] == '\t' || arg[*i] == '\n'
			|| arg[*i] == '\r' || arg[*i] == '\v' || arg[*i] == '\f'))
		(*i)++;
	if (arg[*i] == '-' || arg[*i] == '+')
	{
		if (arg[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static void	chk_nr_vld(const char *arg, int i, unsigned long long *val,
		int digit)
{
	while (arg[i] && ft_isdigit(arg[i]))
	{
		digit = arg[i] - '0';
		if (*val > (ULLONG_MAX - digit) / 10)
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd((char *)arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		*val = *val * 10 + digit;
		i++;
	}
	while (arg[i])
	{
		if (!is_whitespace(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd((char *)arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		i++;
	}
}

static void	validate_digits(const char *arg, int i, int sign)
{
	unsigned long long	val;
	int					digit;

	val = 0;
	digit = 0;
	chk_nr_vld(arg, i, &val, digit);
	if ((sign == 1 && val > LLONG_MAX) || (sign == -1
			&& val > (unsigned long long)LLONG_MAX + 1))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255);
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
		exit(255);
	}
	validate_digits(arg, i, sign);
	return (0);
}

int	ft_exit(t_command *cmd)
{
	int	exit_status;

	if (!cmd->args[0])
		exit(cmd->shell->last_exit_code);
	if (cmd->args[1] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (validate_exit_arguments(cmd))
		return (1);
	exit_status = ft_atoi(cmd->args[0]) & 255;
	exit(exit_status);
}
