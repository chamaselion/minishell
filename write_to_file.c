#include "minishell.h"

void write_parsed_input_to_file(t_parsed_input *parsed_input, const char *filename)
{
    int fd;
    int i;
    char buffer[1024];
    int len;

    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("Error opening file\n", STDERR_FILENO);
        return;
    }

    for (i = 0; i < parsed_input->token_count; i++)
    {
        write(fd, parsed_input->token[i], ft_strlen(parsed_input->token[i]));
        if (parsed_input->special_char[i].count >= 1)
        {
            len = printf(buffer, sizeof(buffer), " (Special: %d)", parsed_input->special_char[i].type);
            write(fd, buffer, len);
        }
        write(fd, "\n", 1);
    }
    write(fd, "---\n", 4);  // Separator between commands

    close(fd);
}
