/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 19:40:54 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/01 15:39:40 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../minishell.h"

t_env *find_env_var(t_env *env_list, const char *name)
{
	t_env *current = env_list;

	while (current)
	{
		if (strcmp(current->key, name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}

void add_env_var(t_env **env_list, const char *name, const char *value)
{
	t_env *new_var = (t_env *)malloc(sizeof(t_env));
	if (!new_var)
	{
		perror("malloc");
		return;
	}

	new_var->key = strdup(name);
	new_var->value = strdup(value);
	new_var->next = *env_list;
	*env_list = new_var;
}

void ft_cd(t_env **env_list, char *path)
{
	char *old_pwd;
	char *new_pwd;
	t_env *env;

	if (!path || !*path)
	{
		fprintf(stderr, "cd: path is required\n");
		return;
	}

	old_pwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(old_pwd);
		return;
	}

	new_pwd = getcwd(NULL, 0);
	env = find_env_var(*env_list, "OLDPWD");
	if (env)
	{
		free(env->value);
		env->value = old_pwd;
	}
	else
	{
		add_env_var(env_list, "OLDPWD", old_pwd);
	}

	env = find_env_var(*env_list, "PWD");
	if (env)
	{
		free(env->value);
		env->value = new_pwd;
	}
	else
	{
		add_env_var(env_list, "PWD", new_pwd);
	}
}

void print_env_vars(t_env *env_list)
{
	t_env *current = env_list;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int main(void)
{
    t_env *env_list = NULL;

    // Initialize the environment variables list with PWD
    char *initial_pwd = getcwd(NULL, 0);
    add_env_var(&env_list, "PWD", initial_pwd);
    free(initial_pwd);

    char *input;
    char *prompt = "minishell> ";

    while ((input = readline(prompt)) != NULL)
    {
        add_history(input);

        // Split the input into command and arguments
        char *command = strtok(input, " ");
        char *arg = strtok(NULL, " ");

        if (command && strcmp(command, "cd") == 0)
            ft_cd(&env_list, arg);
        else if (command && strcmp(command, "exit") == 0)
        {
            free(input);
            break;
        }
        else
            printf("Unknown command: %s\n", command);
        printf("Updated environment variables:\n");
        print_env_vars(env_list);

        free(input);
    }
    t_env *current = env_list;
    t_env *next;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }

    return 0;
}