#include "minishell.h"

void free_parsed_input(t_parsed_input *parsed_input)
{
    int i;

    if (!parsed_input)
        return;

    if (parsed_input->token)
    {
        for (i = 0; i < parsed_input->token_count; i++)
            free(parsed_input->token[i]);
        free(parsed_input->token);
    }

    if (parsed_input->special_char)
        free(parsed_input->special_char);

    free(parsed_input);
}