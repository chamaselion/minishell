/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:20:38 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/01 13:12:40 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../minishell.h"

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

int	main()
{
	ft_pwd();
	return (0);
}

/*This function could probably be better optimized if it read the pwd and oldpwd from env vars.
I'm gonna implement it once we import them in the main loop of the minishell.*/