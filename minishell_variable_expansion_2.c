/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_variable_expansion_2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaumann <mnaumann@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:23:24 by mnaumann          #+#    #+#             */
/*   Updated: 2025/01/25 16:25:32 by mnaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char handle_quotes(char c, unsigned char quote_mode) 
{
    if (c == '\'' && !quote_mode)
        quote_mode ^= 1;
    else if (c == '"' && !quote_mode)
        quote_mode ^= 2;
    return quote_mode;
}

char *append_char_to_output(char *output_str, char c) 
{
    char temp_char[2] = {c, '\0'};
    char *new_output_str = ft_strjoin(output_str, temp_char);
    return new_output_str;
}

char *handle_dollar_sign(t_dollar_params *params) 
{
    char *var_name;
    char *var_value;

    if (params->str[*params->idx + 1] == '?') {
        params->output_str = ft_strjoin(params->output_str, get_exit_code_str(params->shell));
        *params->idx += 2;
    } else if (ft_isalnum(params->str[*params->idx + 1]) || params->str[*params->idx + 1] == '_') {
        var_name = ft_strdup("");
        (*params->idx)++;
        while (ft_isalnum(params->str[*params->idx]) || params->str[*params->idx] == '_') {
            var_name = append_char_to_output(var_name, params->str[*params->idx]);
            (*params->idx)++;
        }
        var_value = ft_getenv(params->shell->env_vars, var_name);
        if (var_value) {
            params->output_str = ft_strjoin(params->output_str, var_value);
        }
        free(var_name);
    } else {
        params->output_str = append_char_to_output(params->output_str, params->str[*params->idx]);
        (*params->idx)++;
    }
    return params->output_str;
}

char *resolve_variables_str(char *str, t_shell *shell) 
{
    char *output_str;
    unsigned char quote_mode;
    int idx;
    t_dollar_params params;

    output_str = ft_strdup("");
    idx = 0;
    quote_mode = 0;
    params.str = str;
    params.idx = &idx;
    params.output_str = output_str;
    params.shell = shell;
    params.quote_mode = quote_mode;
    while (str[idx] != 0) {
        quote_mode = handle_quotes(str[idx], quote_mode);
        if (str[idx] == '$' && (!quote_mode || (quote_mode & 2))) {
            params.quote_mode = quote_mode;
            output_str = handle_dollar_sign(&params);
        } else {
            output_str = append_char_to_output(output_str, str[idx]);
            idx++;
        }
    }
    return output_str;
}
