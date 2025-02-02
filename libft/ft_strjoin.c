/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:12:01 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/02 20:12:01 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char *s1, char *s2)
{
	char		*r;
	t_size_t	c;
	t_size_t	c1;

	c = 0;
	c1 = 0;
	if (!s1 || !s2)
		return (NULL);
	r = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!r)
		return (free(s1), NULL);
	while (s1[c] != '\0')
	{
		r[c] = s1[c];
		c++;
	}
	while (s2[c1] != '\0')
	{
		r[c + c1] = s2[c1];
		c1++;
	}
	r[c + c1] = '\0';
	return (free(s1), r);
}
