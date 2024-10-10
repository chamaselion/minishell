/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 16:15:06 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/01 13:40:26 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../minishell.h"

void unset(t_env **env_list, const char *key) {
    t_env *current;
    t_env *prev;

	prev = NULL;
	current = *env_list;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL)
                *env_list = current->next;
			else
                prev->next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool is_valid_env_var_key(const char *key)
{
	int i;

	i = 1;
    if (!key || !*key)
        return false;
    if (!isalpha((unsigned char)key[0]) && key[0] != '_')
        return false;
    while (key[i] != '\0')
    {
        if (!isalnum((unsigned char)key[i]) && key[i] != '_')
            return false;
		i++;
    }
    return true;
}


void add_or_update_env_var(t_env **env_list, const char *name, const char *value)
{
    t_env *current;
	t_env *new_var;
	
	current = *env_list;
    while (current)
    {
        if (strcmp(current->key, name) == 0)
        {
            free(current->value);
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }
	new_var = (t_env *)malloc(sizeof(t_env));
    if (!new_var)
        return(perror("Error")); //ft_putstr_fd("Error", STDERR_FILENO);
    new_var->key = strdup(name);
    new_var->value = strdup(value);
    new_var->next = *env_list;
    *env_list = new_var;
}

void fill_env_list(t_env **env_list, char **envp)
{
    char **env;
	char *env_entry;
	char *delimiter;
	const char *name;
	const char *value;
	
	env = envp;
    while (*env != NULL)
    {
        env_entry = strdup(*env);
        delimiter = strchr(env_entry, '=');
        if (delimiter)
        {
            *delimiter = '\0';
            name = env_entry;
            value = delimiter + 1;
            if (is_valid_env_var_key(name))
            {
                add_or_update_env_var(env_list, name, value);
            }
        }
        free(env_entry);
        env++;
    }
}

void ft_export(t_env *env_list)
{
    t_env *current;
	
	current = env_list;
    while (current)
    {
        printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_env *env_list;
	t_env *current;
	t_env *next;
	int i;
	char *arg;
	char *delimeter;
	const char *name;
	const char *value;
	
	env_list = NULL;
    fill_env_list(&env_list, envp);

    i = 1;
    while (i < argc)
    {
        char *arg = strdup(argv[i]);
        char *delimiter = strchr(arg, '=');
        if (delimiter)
        {
            *delimiter = '\0';
        	name = arg;
            value = delimiter + 1;
            if (is_valid_env_var_key(name))
                add_or_update_env_var(&env_list, name, value);
        }
        free(arg);
        i++;
    }
    printf("Exported environment variables:\n");
    ft_export(env_list);
	current = env_list;
	next;
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