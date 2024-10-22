#include "mshell_exec.h"

// Function to handle redirections
void handle_input_redirection(t_token *token)
{
    int fd;
	
	fd = open(token->start, O_RDONLY);
    if (fd < 0)
	{
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void handle_output_redirection(t_token *token)
{
    int fd;

	fd = open(token->start, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
	{
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_append_redirection(t_token *token)
{
    int fd;
	
	fd = open(token->start, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) 
	{
        perror("open");
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
    int temp_fd;
	
	temp_fd = open("/tmp/heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (temp_fd < 0)
	{
        perror("open");
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
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(temp_fd, STDIN_FILENO);
    close(temp_fd);
}

void handle_redirections(t_command *cmd)
{
    t_token *token;
	
	token = (t_token *)cmd->related_to;
    while (token != NULL)
	{
        if (token->role == 5)
		{
            if (strncmp(token->start, "<", token->length) == 0)
			{
                token = token->next;
                handle_input_redirection(token);
            }
			else if (strncmp(token->start, ">", token->length) == 0)
			{
                token = token->next;
                handle_output_redirection(token);
            }
			else if (strncmp(token->start, ">>", token->length) == 0)
			{
                token = token->next;
                handle_append_redirection(token);
            }
			else if (strncmp(token->start, "<<", token->length) == 0)
			{
                token = token->next;
                handle_heredoc_redirection(token);
            }
        }
		else
            token = token->next;
    }
}

// Function to create a new token
t_token *create_token(char *start, int length, int role)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    token->start = start;
    token->length = length;
    token->role = role;
    token->next = NULL;
    token->prev = NULL;
    return token;
}

void link_tokens(t_token *prev, t_token *next)
{
    if (prev) prev->next = next;
    if (next) next->prev = prev;
}

void create_file(const char *filename, const char *content)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(fd, content, strlen(content));
    close(fd);
}

// Main function to test handle_redirections
int main()
{
    // Create necessary input files
    create_file("input.txt", "This is a test input file.\n");

    // Test 1: Output Redirection (>)
    t_token *token1 = create_token("echo", 4, 1);
    t_token *token2 = create_token("hello", 5, 3);
    t_token *token3 = create_token(">", 1, 5);
    t_token *token4 = create_token("output.txt", 10, 3);
    link_tokens(token1, token2);
    link_tokens(token2, token3);
    link_tokens(token3, token4);

    t_command cmd1;
    cmd1.related_to = token1;
    handle_redirections(&cmd1);
    system("echo hello > output.txt");
    printf("Test 1: Check output.txt for 'hello'\n");

    // Test 2: Input Redirection (<)
    t_token *token5 = create_token("cat", 3, 1);
    t_token *token6 = create_token("<", 1, 5);
    t_token *token7 = create_token("input.txt", 9, 3);
    link_tokens(token5, token6);
    link_tokens(token6, token7);

    t_command cmd2;
    cmd2.related_to = token5;
    handle_redirections(&cmd2);
    system("cat < input.txt");
    printf("Test 2: Check input.txt for content\n");
    t_token *token8 = create_token("echo", 4, 1);
    t_token *token9 = create_token("world", 5, 3);
    t_token *token10 = create_token(">>", 2, 5);
    t_token *token11 = create_token("append.txt", 10, 3);
    link_tokens(token8, token9);
    link_tokens(token9, token10);
    link_tokens(token10, token11);
    t_command cmd3;
    cmd3.related_to = token8;
    handle_redirections(&cmd3);
    system("echo world >> append.txt");
    printf("Test 3: Check append.txt for 'world'\n");

    free(token1);
    free(token2);
    free(token3);
    free(token4);
    free(token5);
    free(token6);
    free(token7);
    free(token8);
    free(token9);
    free(token10);
    free(token11);
    return 0;
}
