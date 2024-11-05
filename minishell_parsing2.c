/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:27:50 by mnaumann          #+#    #+#             */
/*   Updated: 2024/11/02 16:27:06 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void add_special_char(t_parsed_input *parsed_input,
    e_special_char type, int position)
{
    parsed_input->special_char[parsed_input->special_char_count].type = type;
    parsed_input->special_char[parsed_input->special_char_count].position = 
        position;
    parsed_input->special_char_count++;
}

static e_special_char get_quote_type(char quote_char)
{
    if (quote_char == '\'')
        return (QUOTE);
    return (DOUBLE_QUOTE);
}