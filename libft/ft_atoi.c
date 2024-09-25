/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 18:14:50 by bszikora          #+#    #+#             */
/*   Updated: 2024/03/09 18:55:42 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *t)
{
	int	r;
	int	c;
	int	s;

	c = 0;
	r = 0;
	s = 1;
	while (t[c] == ' ' || t[c] == '\t' || t[c] == '\r' || t[c] == '\n'
		|| t[c] == '\v' || t[c] == '\f')
	{
		c++;
	}
	if (t[c] == '-' || t[c] == '+')
	{
		if (t[c] == '-')
			s = -1;
		c++;
	}
	while (t[c] >= '0' && t[c] <= '9')
	{
		r = r * 10 + (t[c] - '0');
		c++;
	}
	return (r * s);
}
