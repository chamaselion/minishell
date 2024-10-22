#include "mshell_exec.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, strlen(s));
}

char *search_command(const char *command)
{
	char *path;
	char *dir;
	char full_path[4096];
	char *result;
	char *path_env;
	
	path_env = getenv("PATH");
	if (!path_env)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	path = strdup(path_env);
	if (!path)
		return (ft_putstr_fd("Error", STDERR_FILENO), NULL);
	dir = strtok(path, ":");
	while (dir != NULL)
	{
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (result = strdup(full_path), free(path), result);
		dir = strtok(NULL, ":");
	}
	return (free(path), NULL);
}

void execute_command(const char *full_path, t_command *cmd)
{
    char *argv[cmd->arg_count + 2];
	int i;

    argv[0] = (char *)full_path;
	i = 0;
    while (i < cmd->arg_count)
    {
        argv[i + 1] = cmd->args[i];
		i++;
    }
    argv[cmd->arg_count + 1] = NULL;
    if (execve(full_path, argv, NULL) == -1)
    {
    	ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}

void search_and_execute(t_command *cmd)
{
    char *full_path;
	
	full_path = search_command(cmd->command);
    if (full_path)
    {
        execute_command(full_path, cmd);
        free(full_path);
    }
    else
    {
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}

int main()
{
    // Create a sample command
    t_command cmd;
    cmd.command = "git";
    cmd.args = (char *[]){"status", NULL};
    cmd.arg_count = 1;
    cmd.next = NULL;

    // Search and execute the command
    search_and_execute(&cmd);

    return 0;
}