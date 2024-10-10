#include "mshell_exec.h"

void	ft_echo(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_cd(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_pwd(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_export(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_unset(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_env(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}
void	ft_exit(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	printf("\n%s\n%s\n", command, args[0]);
}