/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ft_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:04:11 by bszikora          #+#    #+#             */
/*   Updated: 2025/01/20 19:04:50 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	ft_pwd(void)
{
	char *pwd;
	pwd = malloc(sizeof(char) * (32768 + 1));   //Maximum value for a path on ntfs file system, alternative to using
												//PATH_MAX which seemed unreliable considering it's defined as a different value in many systems
												//and often does not actually refer to the largest possible path that can be created with tricky ways
	if (getcwd(pwd, 32768))
		return (printf("%s\n", pwd), 0);
	else
		return (1);
}
