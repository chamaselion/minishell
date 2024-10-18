/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec_cases.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:37:57 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/18 11:11:00 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mshell_exec.h"

void handle_input_redirection(t_token *token)
{
    int fd = open(token->start, O_RDONLY);
    if (fd < 0)
	{
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void handle_output_redirection(t_token *token)
{
    int fd = open(token->start, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
	{
    	ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_append_redirection(t_token *token)
{
    int fd = open(token->start, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) 
	{
		ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_heredoc_redirection(t_token *token)
{
    char *delimiter = token->start;
    char buffer[1024];
    int bytes_read;
    int temp_fd = open("/tmp/heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (temp_fd < 0)
	{
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
	{
        buffer[bytes_read] = '\0';
        if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 && buffer[strlen(delimiter)] == '\n') 
            break;
        write(temp_fd, buffer, bytes_read);
    }
    close(temp_fd);
    temp_fd = open("/tmp/heredoc_tmp", O_RDONLY);
    if (temp_fd < 0)
	{
        ft_putstr_fd("Error", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    dup2(temp_fd, STDIN_FILENO);
    close(temp_fd);
}

void process_redirection(t_token **token)
{
    if (strncmp((*token)->start, "<", (*token)->length) == 0)
	{
        *token = (*token)->next;
        handle_input_redirection(*token);
    }
	else if (strncmp((*token)->start, ">", (*token)->length) == 0)
	{
        *token = (*token)->next;
        handle_output_redirection(*token);
    }
	else if (strncmp((*token)->start, ">>", (*token)->length) == 0)
	{
        *token = (*token)->next;
        handle_append_redirection(*token);
    }
	else if (strncmp((*token)->start, "<<", (*token)->length) == 0)
	{
        *token = (*token)->next;
        handle_heredoc_redirection(*token);
    }
}

void handle_redirections(t_command *cmd) {
    t_token *token;
	
	token = (t_token *)cmd->related_to;
    while (token != NULL)
	{
        if (token->role == 5)
            process_redirection(&token);
		else
            token = token->next;
    }
}
