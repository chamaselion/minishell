#include "mshell_exec.h"

char	*ft_echo(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_cd(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_pwd(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_export(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_unset(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_env(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}
char	*ft_exit(t_command *cmd, t_shell *shell)
{
	char *command;
    char **args;

	command = cmd->command;
	args = cmd->args;

	strcpy(cmd->output, command);
    strcat(cmd->output, args[0]);
}