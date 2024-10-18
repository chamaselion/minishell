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

typedef struct s_token {
    char *start;
    int length;
    int role; //0 = nothing special, 1 = command, 2 = executable, 3 = argument, 4 = string/variable to expand/read, 5 = redirect, 6 = pipe, -1 = error (not found)
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
	void *related_to;
	int	relation_type
    struct s_command *next;
    int append;
    int priority;
} t_command;

typedef struct s_shell
{
    t_command *commands;
    char cwd[32768];
} t_shell;

typedef void (*func_ptr)(t_command *, t_shell *);

void	ft_echo(t_command *cmd, t_shell *shell);
void	ft_cd(t_command *cmd, t_shell *shell);
void	ft_pwd(t_command *cmd, t_shell *shell);
void	ft_export(t_command *cmd, t_shell *shell);
void	ft_unset(t_command *cmd, t_shell *shell);
void	ft_env(t_command *cmd, t_shell *shell);
void	ft_exit(t_command *cmd, t_shell *shell);

# endif;