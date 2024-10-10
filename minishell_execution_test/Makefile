CC = gcc
CFLAGS = -Wall -Wextra
SRC = minishell_exec.c minishell_fts.c
OBJ = $(addprefix objects/, $(SRC:.c=.o))
NAME = minishell
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

objects/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re