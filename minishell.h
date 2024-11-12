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
    DASH = '-',
    T_SPACE = ' ',
    T_TAB = '\t',
    T_NEWLINE = '\n',
    END_OF_FILE = '\0',
} e_special_char;

typedef enum e_token_role {
    ROLE_DELIMITER = 0,        // quotes, spaces..."
    ROLE_BUILTIN = 1,       // Built-in commands (echo, cd, pwd, etc.)
    ROLE_EXECUTABLE = 2,    // External executables (found in PATH)
    ROLE_ARGUMENT = 3,      // Arguments to commands
    ROLE_ASSIGNMENTOPERATOR = 4,      // Variable to be expanded ($VAR)
    ROLE_REDIRECT = 5,        // Command options/flags (-n)
    ROLE_PIPE = 6,     // Quotes, pipes, redirections
    ROLE_ERROR = -1,        // For tokens that can't be properly categorized
    ROLE_DEFAULT = -2      // New initial value
} t_token_role;

typedef enum e_cmd_type {
    CMD_NONE = 0,
    CMD_BUILTIN = 1,
    CMD_EXECUTABLE = 2,
    CMD_PIPE = 3,
    CMD_REDIR = 4
} t_cmd_type;

typedef enum e_builtin {
    echo = 0,
    cd = 1,
    pwd = 2,
    export = 3,
    unset = 4,
    env = 5,
    exit_cmd = 6
} t_builtin;

typedef struct s_special_char_struct
{
    e_special_char type;
    int count; // Number of occurrences - use it to enter interactive mode!! see below for new approach
    int position; // Position in the input string
} t_special_char_struct;

typedef struct s_token {
    char *start;
    int length;
    int role; //0 = nothing special, 1 = command, 2 = executable, 3 = argument, 4 = string/variable to expand/read, -1 = error (not found)
    int quote_state;
    struct s_token *next;
    struct s_token *prev;
} t_token;

typedef enum e_quote_state {
    NO_QUOTE = 0,
    WITHIN_SINGLE_QUOTE = 1,
    WITHIN_DOUBLE_QUOTE = 2,
    UNCLOSED_SINGLE_QUOTE = 3,
    UNCLOSED_DOUBLE_QUOTE = 4
} t_quote_state;

typedef struct s_env_var {
    char *string;
    char *key;
    char *value;
    int length;
    int format; //depending on if called via export, echo or env, 0 = export, 1 = env, 2 = echo, 3= echo -n
    struct s_env_var *next;
    struct s_env_var *prev;
} t_env_var;

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
    t_cmd_type type;
} t_command;

typedef struct s_parsed_input
{
    t_token *token;
    t_special_char_struct *special_char;
    t_command **commands;
    int token_count;
    t_token *last_token;
    int special_char_count;
    char *delimiters;
    int save_history; // 1 = save, 0 = don't save (used as boolean)
    int interactive_mode; // 1 = interactive, 0 = non-interactive (used as boolean)
    t_env_var *env;
} t_parsed_input;

typedef struct s_shell
{
    t_command *commands;
    char cwd[MAX_PATH];
} t_shell;


//Init:
void init_parsed_input(t_parsed_input *parsed_input);
void init_special_char_handling(t_special_char_struct *special_char);
void init_command(t_command *cmd);
void init_token(t_token *token);
int is_special_char(char c);


//nu:
int check_unclosed_quotes(t_parsed_input *parsed_input);
int refine_tokens(t_parsed_input *parsed_input);
int initial_tokenization(t_parsed_input *parsed_input, char *input);
int is_delimiter(char c);
int is_quote_char(char c);
int toggle_quote_state(char c, int quote_state);
int assign_roles(t_parsed_input *parsed_input);
void assign_role(t_token *token, int *expect_command);
int is_builtin_command(const char *cmd);
void skip_whitespace(char **input);


int		assign_roles(t_parsed_input *parsed_input);
void	assign_role(t_token *token, int *expect_command);
int add_token(t_parsed_input *parsed_input, char *start, int length, t_quote_state quote_state);
int		process_quotes(t_parsed_input *parsed_input);
int		handle_input(char *input);
int split_and_add_token(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state);
void remove_and_update_token(t_token **token_ptr, t_parsed_input *parsed_input, t_token *token);
int get_quote_state(char c);
int process_quoted_content(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state);
int is_matching_quote(char c, int state);
int add_segment(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state);
int is_redirection(char *str);
int free_and_fail(char *content, char *message);
int add_variable_tokens(t_parsed_input *parsed_input, char *str, int *i,
                        t_token **prev);
int free_and_return(char *content, int ret);
int add_token_segment(t_parsed_input *parsed_input, char *content, t_token *prev, int quote_state);
void remove_token(t_parsed_input *parsed_input, t_token *token);
char *strip_quotes(const char *str, int *quote_state);
void add_new_token(t_token **head, t_token **tail, char *start, int length, int quote_state);
void assign_role_based_on_position(t_token *token, int *expect_command);
void insert_token(t_parsed_input *parsed_input, t_token *new_token, t_token *prev);
void insert_token_after(t_parsed_input *parsed_input, t_token *prev, t_token *new_token);
int add_subtoken(t_parsed_input *parsed_input, t_token **prev, char *start, int length, int quote_state);
int expand_variables_in_token(t_parsed_input *parsed_input, t_token *token);

int process_double_quote_token(t_parsed_input *parsed_input, t_token *token);
int process_no_quote_token(t_parsed_input *parsed_input, t_token *token);




void write_parsed_input_to_file(t_parsed_input *parsed_input, const char *filename);


void print_tokens(t_parsed_input *parsed_input);

// Utils:
char *ft_strtok(char *str, const char *delimiters);
int ft_atoi(const char *str);
char *ft_strchr(const char *s, int c);
char *ft_strncpy(char *dest, const char *src, size_t n);
char *ft_strtok_r(char *str, const char *delim, char **saveptr);
char *ft_strjoin_and_free(char *s1, char *s2);
int ft_strcmp(const char *s1, const char *s2);

// Input handling:
int handle_input(char *input);

// Signal handling:
void setup_signal_handling();

// Freeing:
void free_parsed_input(t_parsed_input *parsed_input);
void free_command(t_command *cmd);
void free_commands(t_command *cmd);
void free_tokens(t_token *token);

#endif