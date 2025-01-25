/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 12:36:13 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/26 00:09:30 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	*ft_realloc(void *ptr, int old_size, int new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
	{
		ft_putstr_fd("Error allocating memory", STDERR_FILENO);
		return (NULL);
	}
	if (ptr != NULL)
	{
		if (old_size < new_size)
			ft_memmove(new_ptr, ptr, old_size);
		else
			ft_memmove(new_ptr, ptr, new_size);
		free(ptr);
	}
	return (new_ptr);
}

char *ft_strcat(char *dest, const char *src)
{
	char	*dest_ptr;

	dest_ptr = dest;
	while (*dest_ptr != '\0')
	{
		dest_ptr++;
	}
	while (*src != '\0')
	{
		*dest_ptr = *src;
		dest_ptr++;
		src++;
	}
	*dest_ptr = '\0';
	return (dest);
}