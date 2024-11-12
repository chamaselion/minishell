/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:26:43 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/11 16:57:18 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_delimiter(char c)
{
    return (c == ' ' || c == '\t' || c == '\0');
}

int split_and_add_token(t_parsed_input *parsed_input, char *content, t_token **prev, int quote_state)
{
    char *str = content;
    int i = -1, start = 0;
    while (str[++i])
    {
        if (str[i] == '$')
        {
            if (i > start && !add_segment(parsed_input, ft_substr(str, start, i - start), prev, quote_state))
                return free_and_return(content, 0);
            if (!add_variable_tokens(parsed_input, str, &i, prev))
                return free_and_return(content, 0);
            start = i + 1;
        }
    }
    if (i > start && !add_segment(parsed_input, ft_substr(str, start, i - start), prev, quote_state))
        return free_and_return(content, 0);
    free(content);
    return 1;
}

int add_variable_tokens(t_parsed_input *parsed_input, char *str, int *i,
                        t_token **prev)
{
    if (!add_segment(parsed_input, ft_strdup("$"), prev, NO_QUOTE))
        return 0;
    (*prev)->role = ROLE_ASSIGNMENTOPERATOR;
    int start = *i + 1;
    (*i)++;
    while (str[*i] && ft_isalnum(str[*i]))
        (*i)++;
    if (*i > start)
    {
        if (!add_segment(parsed_input, ft_substr(str, start, *i - start),
                         prev, NO_QUOTE))
            return 0;
        (*prev)->role = ROLE_ASSIGNMENTOPERATOR;
    }
    else
        (*prev)->role = ROLE_ARGUMENT;
    (*i)--;
    return 1;
}


void insert_token(t_parsed_input *parsed_input, t_token *new_token, t_token *prev)
{
    if (prev)
    {
        new_token->next = prev->next;
        if (prev->next)
            prev->next->prev = new_token;
        prev->next = new_token;
        new_token->prev = prev;
    }
    else
    {
        new_token->next = parsed_input->token;
        if (parsed_input->token)
            parsed_input->token->prev = new_token;
        parsed_input->token = new_token;
    }
}

int add_token_segment(t_parsed_input *parsed_input, char *content, t_token *prev, int quote_state)
{
    t_token *new_token;

    if (!content || !*content)
    {
        free(content);
        return 1;
    }
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (free(content), ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO), 0);

    init_token(new_token);
    new_token->start = content;
    new_token->length = ft_strlen(content);
    new_token->quote_state = quote_state;
    new_token->role = ROLE_DEFAULT;

    insert_token(parsed_input, new_token, prev);
    return 1;
}
