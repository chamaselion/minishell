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

#define MAX_ARGS 64
#define MAX_PATH 1024

// Special characters:
typedef enum e_special_char
{
    DOLLAR = '$',
    PIPE = '|',
    REDIR_OUT = '>',
    REDIR_IN = '<',
    QUOTE = '\'',
    DOUBLE_QUOTE = '"',
    DASH = '-',
    T_SPACE = ' ',
    T_TAB = '\t',
    T_NEWLINE = '\n',
    END_OF_FILE = '\0',
} e_special_char;

typedef struct s_special_char_struct
{
    e_special_char type;
    int count; // Number of occurrences - use it to enter interactive mode!!
    int position; // Position in the input string
} t_special_char_struct;

typedef struct s_token {
    char *start;
    int length;
    int role; //0 = nothing special, 1 = command, 2 = executable, 3 = argument, 4 = string/variable to expand/read, -1 = error (not found)
    struct s_token *next;
    struct s_token *prev;
} t_token;

typedef struct s_command
{
    char *command;
    char *input;
    char **args;
    int arg_count;
    char *output;
    struct s_command *next;
    int append;
    int priority;
} t_command;

typedef struct s_parsed_input
{
    t_token *token;
    t_special_char_struct *special_char;
    t_command **commands;
    int token_count;
    int special_char_count;
    char *delimiters;
} t_parsed_input;

typedef struct s_shell
{
    t_command *commands;
    char cwd[MAX_PATH];
} t_shell;

void init_parsed_input(t_parsed_input *parsed_input);
void init_special_char_handling(t_special_char_struct *special_char);
void init_command(t_command *cmd);
void init_token(t_token *token);
int is_special_char(char c);

// Parsing:
//t_parsed_input *parsing(char *input);
void write_parsed_input_to_file(t_parsed_input *parsed_input, const char *filename);
//void process_special_char(char *token, t_special_char_struct *special_char);

// Utils:
char *ft_strtok(char *str, const char *delimiters);
int ft_atoi(const char *str);
char *ft_strchr(const char *s, int c);
char *ft_strncpy(char *dest, const char *src, size_t n);
char *ft_strtok_r(char *str, const char *delim, char **saveptr);

// Input handling:
int handle_input(char *input);

// Signal handling:
void setup_signal_handling();

// Freeing:
void free_parsed_input(t_parsed_input *parsed_input);
void free_command(t_command *cmd);
void free_commands(t_command *cmd);

#endif