/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:42:16 by mnaumann          #+#    #+#             */
/*   Updated: 2024/09/19 11:34:02 by root             ###   ########.fr       */
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

// Special characters:
typedef enum e_special_char
{
	DOLLAR = '$',
	PIPE = '|',
	REDIRECT = '>',
	APPEND = '>>',
	INPUT = '<',
	QUOTE = '\'',
	DOUBLE_QUOTE = '"',
	BACKSLASH = '\\',
	T_SPACE = ' ',
	T_TAB = '\t',
	T_NEWLINE = '\n',
	END_OF_FILE = '\0',
}	e_special_char;

typedef struct s_special_char_struct
{
	char			**special_char;
	void			(*function)(void **str);
	int				status; // -1 = not found, 0 = found, 1 = found 2 (or more = n -1) times, 
					//e.g. quotes opened and closed
	e_special_char	type;
	
}	t_special_char_struct;

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

typedef struct s_parsed_input
{
	void					**token;
	t_special_char_struct	*special_char;
	void					**commands;
	int						token_count;
	e_special_char			*delimiters;
}	t_parsed_input;

int		is_special_char(char *str, e_special_char type);

// Initialization:
void init_parsed_input(t_parsed_input *parsed_input);
void init_special_char_handling(t_special_char_struct *special_char);

// Parsing:

// Utils:
char	**ft_strtok(char *str, const char *delimiters); //add possibility to change struct values
int		ft_atoi(const char *str);			
char	*ft_strchr(const char *s, int c); //see above
char	*ft_strncpy(char *dest, const char *src, size_t n); //add to token and token count
char	*ft_strtok_r(char *str, const char *delim, char **saveptr);

// Input handling:
int handle_input(char *input);

// Signal handlind
void setup_signal_handling();

#endif