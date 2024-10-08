/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:12:45 by root              #+#    #+#             */
/*   Updated: 2024/10/07 18:40:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//different approach seems to handle it easier

/*int is_special_char(char *str, e_special_char type)
{
    int count = 0;

    if (str == NULL || *str == '\0')
        return -1;

    // Handle double and single quotes
    if (type == QUOTE || type == DOUBLE_QUOTE)
    {
        char *ptr = str;
        while ((ptr = ft_strchr(ptr, (char)type)) != NULL)
        {
            count++;
            ptr++;
        }
        return count;
    }

    // Handle << and >>
    if (type == REDIR_IN && str[0] == '<' && str[1] == '<')
        return 2;
    if (type == REDIR_OUT && str[0] == '>' && str[1] == '>')
        return 2;

    // Handle single character special types
    if (*str == (char)type)
    {
        count = 1;
        while (*(++str) == (char)type)
            count++;
        return count;
    }

    // Handle $ within double quotes
    if (type == DOLLAR && *str == DOLLAR)
    {
        if (str[1] == '?')
            return 127; // Return the last exit code
        return 1;
    }

    return -1;
}*/
int is_special_char(char c)
{
    return (c == '|' || c == '>' || c == '<' || c == '$' || c == '\'' || c == '"');
}

char *ft_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *end;

    if (str == NULL)
        str = *saveptr;
    while (*str && ft_strchr(delim, *str))
        str++;
    if (*str == '\0')
    {
        *saveptr = str;
        return (NULL);
    }
    end = str;
    while (*end && !ft_strchr(delim, *end))
        end++;
    if (*end == '\0')
    {
        *saveptr = end;
        return (str);
    }
    *end = '\0';
    *saveptr = end + 1;
    return (str);
}

char *ft_strtok(char *str, const char *delim)
{
    static char *last;

    return (ft_strtok_r(str, delim, &last));
}

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}

int ft_itoa_base(int value, int base, char *buffer, int size)
{
    char *digits = "0123456789abcdef";
    char tmp[32];
    int i = 0, j = 0;
    int negative = 0;

    if (value < 0)
    {
        negative = 1;
        value = -value;
    }

    do
    {
        tmp[i++] = digits[value % base];
        value /= base;
    } while (value && i < 32);

    if (negative && j < size - 1)
        buffer[j++] = '-';

    while (i > 0 && j < size - 1)
        buffer[j++] = tmp[--i];

    buffer[j] = '\0';
    return j;
}