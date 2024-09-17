/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:12:45 by root              #+#    #+#             */
/*   Updated: 2024/09/16 18:41:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char *str, e_special_char type)
{
	if (str == NULL || special_char == NULL)
		return (0);
	return (*str == (char)type);
// implement the rest of the function, recognizing the special characters
// return (1) if character is found, (0) if not, used lika a bool later on in combo with the status
// CAUTION with multichar special characters, e.g. '>>' or '&&' and escape char "\"
}