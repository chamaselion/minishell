/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:42:16 by mnaumann          #+#    #+#             */
/*   Updated: 2024/09/16 18:47:02 by root             ###   ########.fr       */
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

typedef struct s_special_char_struct
{
	char			*char;
	void			(*function)(void **str);
	int				status; // -1 = not found, 0 = found, 1 = found 2 (or more = n -1) times, 
					//e.g. quotes opened and closed
	e_special_char	type;
	
}	t_special_char_struct;

typedef struct s_parsed_input
{
	void	**token;
	t_special_char
	void	**commands;
	int		token_count;
}	t_parsed_input;

// Special characters:
typedef enum e_special_char
{
	PIPE = '|',
	SEMICOLON = ';',
	REDIRECT = '>',
	APPEND = '>>',
	INPUT = '<',
	QUOTE = '\'',
	DOUBLE_QUOTE = '"',
	BACKTICK = '`',
	BACKSLASH = '\\',
	SPACE = ' ',
	TAB = '\t',
	NEWLINE = '\n',
	CARRIAGE_RETURN = '\r',
	END_OF_FILE = '\0',
}	e_special_char;

void is_special_char(char *str, t_special_char *special_char);

// Initialization:
void init_parsed_input(t_parsed_input *parsed_input);
void init_special_char_handling(t_special_char_struct *special_char);

// Parsing:
char **ft_strtok(char *str, const char *delimiters); // https://cplusplus.com/reference/cstring/strtok/

// Input handling:
int handle_input(char *input);

// Signal handlind
void setup_signal_handling();

#endif