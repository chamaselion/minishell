/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:42:16 by mnaumann          #+#    #+#             */
/*   Updated: 2024/09/11 21:46:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include "libft/libft.h"

typedef struct s_env
{
	char			*key;
	void			*value;
	struct s_env	*next;
	int				status;
}	t_env;

typedef struct s_command
{
	void	*command;
	void	*input;
	void	**args;
	void	*output;
	void	*pipe;
}	t_command;

typedef struct s_special_char
{
	char	*char;
	void	(*function)(void **str);
	int		status; // -1 = not found, 0 = found, 1 = found 2 (or more = n -1) times, 
					//e.g. quotes opened and closed
	
}	t_special_char;

typedef struct s_parsed_input
{
	void	**input;
	char	*delimiters;
	void	**commands;
}	t_parsed_input;

// Parsing:
char **strtok(char *str, const char *delimiters); // https://cplusplus.com/reference/cstring/strtok/

// Input handling:
int handle_input(char *input);

// Signal handlind
void setup_signal_handling();

#endif