# ifndef MSHELLEXEC
# define MSHELLEXEC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_command
{
    char *command;
    char *input;
    char **args;
    int arg_count;
    char *output;
    struct s_command *next;
    int append;
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

# endif