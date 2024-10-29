# ifndef MSHELLEXEC
# define MSHELLEXEC

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_token
{
    char *start; //The string containing the token
    int length; //Length of the string containing the token
    int role; //0 = delimeter, 1 = command, 2 = executable, 3 = argument, 4 = Assignment operator, 5 = redirect, 6 = pipe, -1 = error (not found)
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
	struct s_command *related_to;
    int	relation_type; //0 = delimeter, 1 = command, 2 = executable, 3 = argument, 4 = Assignment operator, 5 = redirect, 6 = pipe, -1 = error (not found)
    struct s_command *next;
    int is_internal; // 0 = Not internal, 1 =  Is internal function
	t_token	*input_redirection;
    t_token	*output_redirection;
    t_token	*append_redirection;
    t_token	*heredoc_redirection;
} t_command;

typedef struct s_shell
{
    t_command *commands;
    char cwd[32768];
} t_shell;

typedef void (*func_ptr)(t_command *, t_shell *);

//void	ft_echo(t_command *cmd, t_shell *shell);
//void	ft_cd(t_command *cmd, t_shell *shell);
//void	ft_pwd(t_command *cmd, t_shell *shell);
//void	ft_export(t_command *cmd, t_shell *shell);
//void	ft_unset(t_command *cmd, t_shell *shell);
//void	ft_env(t_command *cmd, t_shell *shell);
//void	ft_exit(t_command *cmd, t_shell *shell);


char	*ft_cd(t_command *cmd);
char	*ft_echo(t_command *cmd);
char	*ft_exit(t_command *cmd);
char	*ft_export(t_command *cmd);
char	*ft_pwd(t_command *cmd);
void handle_pipes(t_command *cmd);


# endif