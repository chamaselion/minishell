/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:42:16 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/22 12:58:04 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

# define MAX_ARGS 64
# define MAX_PATH 1024


extern int g_received_signal;
typedef struct  s_command t_command;

typedef enum e_token_syntax_state
{
	SYNTAX_VALID = 0,
	SYNTAX_INCOMPLETE = 1,
	SYNTAX_ERROR = 2
}   t_token_syntax_state;

typedef enum e_token_role
{
	ROLE_DELIMITER = 0,
	ROLE_BUILTIN = 1,
	ROLE_EXECUTABLE = 2,
	ROLE_ARGUMENT = 3,
	ROLE_VARIABLE = 4,
	ROLE_REDIRECT = 5,
	ROLE_PIPE = 6,
	ROLE_ERROR = -1,
	ROLE_DEFAULT = -2
}   t_token_role;

typedef enum e_builtin
{
	echo = 0,
	cd = 1,
	pwd = 2,
	export = 3,
	unset = 4,
	env = 5,
	exit_cmd = 6
}   t_builtin;

typedef enum e_redir_type
{
	REDIR_NONE = 0,
	REDIR_IN = 1,
	REDIR_OUT = 2,
	REDIR_APPEND = 3,
	REDIR_HEREDOC = 4
} t_redir_type;

typedef struct s_token
{
	char            *content;
	int             role;
	int             quote_state;
	int             position;
	int             command_expected;
	int             syntax_state;
	struct s_token  *next;
	struct s_token  *prev;
} t_token;

typedef enum e_quote_state
{
	NO_QUOTE = 0,
	WITHIN_SINGLE_QUOTE = 1,
	WITHIN_DOUBLE_QUOTE = 2,
	UNCLOSED_SINGLE_QUOTE = 3,
	UNCLOSED_DOUBLE_QUOTE = 4
} t_quote_state;

typedef struct s_env_var
{
	char *string;
	char *key;
	char *value;
	int length;
	int format;
	struct s_env_var *next;
	struct s_env_var *prev;
} t_env_var;

typedef struct s_shell
{
	t_command *commands;
	char cwd[MAX_PATH];
	t_env_var *env_vars;
	int last_exit_code;
} t_shell;

struct s_command
{
	char *command;
	char *input;
	char **args;
	int arg_count;
	char *output;
	t_command *related_to;
	int relation_type;
	t_command *next;
	int is_internal;
	t_token *input_redirection;
	t_token *output_redirection;
	t_token *append_redirection;
	t_token *heredoc_redirection;
	t_shell *shell;
};

typedef struct s_raw_token
{
	char *segment;
	int quote_state;
	int position;
	struct s_raw_token *next;
	struct s_raw_token *prev;
} t_raw_token;

int	ft_echo(t_command *cmd);
int	ft_env(t_command *cmd);
int	ft_export(t_command *cmd);
int	ft_unset(t_command *cmd);
int		ft_pwd(void);
int	ft_cd(t_command *cmd);
int ft_exit(t_command *cmd);

void set_or_create_env_var(t_env_var **env_vars, const char *key, const char *value);

// Init:
void	init_token(t_token *token, t_raw_token *t_raw_token);
void	order_extra(void);
void	check_order(t_token *tokens);
void 	init_shell(t_shell *shell, t_env_var *env_vars);

// env_var:
t_env_var	*init_env_vars(char **envp);
int     	identify_env_var(char *str);
char		*expand_env_variable(const char *var_name);
void		append_variable_value(const char *var_name, char **write_ptr);
void		update_exit_code(t_shell *shell, int exit_code);
char		*get_exit_code_str(t_shell *shell);
char* get_env_var(const char *key, t_env_var *env_vars);

// Parsing:
t_raw_token *handle_input(char *input, t_shell* shell);
t_token     *allocate_token(int length);
void        fill_token_fields(t_token *token, char *start, int length,
		        int quote_state);
int     is_command_expected(t_token *prev_token);
t_raw_token *create_raw_token(const char *segment, t_quote_state quote_state,
		int position);
void        append_raw_token(t_raw_token **first, t_raw_token **last,
		t_raw_token *new_token);
void        append_character(char c, char **write_ptr);
const char  *extract_variable_name(const char *current, char *var_name);
char        *expand_double_quote_content(const char *content);
t_token     *convert_raw_token(t_raw_token *raw_token);
int         is_raw_token_list_empty(t_raw_token *raw_token_head);
void        link_token_to_list(t_token **new_head, t_token **current_new,
		t_token *new_token);
t_token     *convert_raw_token_list(t_raw_token *raw_token_head);
void        assign_token_role(t_token *token_list);
void        handle_command_token(t_token *token);
void        handle_redirect_token(t_token *token);
void        handle_pipe_token(t_token *token);
int         validate_token_syntax(t_token *token_list);
t_token	    *finalizing_token_list(t_token *token_list);
void	    remove_token(t_token **head, t_token *token);
void	    print_token_list(t_token *token_list);

// Quote handling:
t_token	    *pop_quotemark_tokens(t_token **token_list);
void	    single_quote_wrapper(const char **input, int *pos, t_raw_token **first,
		t_raw_token **last);
void	    double_quote_wrapper(const char **input, int *pos, t_raw_token **first,
		t_raw_token **last);
t_raw_token	*handle_double_quote_segment(const char **input, int *pos);
t_raw_token	*handle_single_quote_segment(const char **input, int *pos);
t_raw_token	*handle_single_quote_mark(int *pos);
t_raw_token	*handle_double_quote_mark(int *pos);
t_raw_token	*handle_non_quote_segment(const char **input, int *pos);
void	check_for_unclosed(t_token *token_list);
char **purge_quotes_from_args(t_command *cmd);
char *purge_quotes_from_arg(char *args);

// Utils:
char	*ft_strtok(char *str, const char *delimiters);
int	ft_atoi(const char *str);
char	*ft_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strtok_r(char *str, const char *delim, char **saveptr);
char	*ft_strjoin_and_free(char *s1, char *s2);
int     ft_strcmp(const char *s1, const char *s2);
int     is_quote_char(char c);
int     get_quote_type(char c);
int     is_whitespace(char c);
int     is_builtin_command(const char *cmd);
int     is_redirection(char *str);
int     is_pipe(char *str);
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strcpy(char *dst, const char *src);
char	*skip_whitespace(char *input);
int     is_valid_env_var_name(const char *str);
char **convertEnvironmentToArray(t_env_var* environment);
void free_split_array(char **array);
int ft_lstsize(t_env_var *lst);
char *ft_strjoin_and_free2(char *s1, char *s2);

// Command filler
t_command	*create_new_command(void);
int         expected_command_counter(t_token *token);
t_command	*create_command_list(t_token *tokens);
int         allocate_args(t_command *current_cmd, t_token *ct);
void        remove_last_empty_command(t_command *head_cmd, t_command *current_cmd);
int         process_tokens(t_token *ct, t_command *current_cmd);
int         fill_command_from_tokens(t_token *tokens, t_command **cmd);
void        link_commands_and_tokens(t_token *tokens, t_command *cmd);

// Execution
void	handle_ft_command(t_command *cmd);
char	*search_command(const char *command, t_env_var *env_var);
char	**construct_exec_args(t_command *cmd);
void	execute_command(t_command *cmd, char **exec_args);

// Piping
void	setup_redirection(t_command *cmd, int in_fd, int pipefd[2]);
void	create_pipe(int pipefd[2]);
pid_t	fork_process(void);
void	handle_child_process(t_command *cmd, int in_fd, int pipefd[2]);
void	handle_parent_process(t_command *cmd, int *in_fd, int pipefd[2]);
void	handle_pipes(t_command *cmd);

// Redirection
void	handle_input_redirection(t_command *cmd);
void	handle_output_redirection(t_command *cmd);
void	handle_append_redirection(t_command *cmd);
void	handle_pipe_redirection(t_command *cmd, int pipefd[2]);
void	handle_heredoc_redirection(t_command *cmd);

// Signal handling:
void	setup_signal_handling(void);

// Freeing:
void	free_command(t_command *cmd);
void	free_commands(t_command *cmd);
void	free_tokens(t_token *token);
void	free_raw_tokens(t_raw_token *first_token);
void	free_env_vars(t_env_var *env_vars);
void	free_shell(t_shell *shell);

//debugging:
void	print_raw_tokens(t_raw_token *first_token);
void	print_tokens(t_token *first_token);
void	print_commands(t_command *first_command);

#endif
