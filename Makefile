CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC = src/minishell_loop.c\
 	src/minishell_loop_utils.c\
    src/minishell_tokenizer2.c\
    src/minishell_tokenizer3.c\
    src/minishell_tokenizer.c\
	src/minishell_concatenation.c\
    src/minishell_variable_expansion.c\
	src/minishell_variable_expansion2.c\
    src/minishell_roles_cmdexpected.c\
	src/minishell_roles_cmdexpected2.c\
    src/minishell_syntax.c\
    src/minishell_signal.c\
    src/minishell_quote_handling.c\
	src/minishell_quote_handling2.c\
    src/minishell_free_and_exit.c\
    src/minishell_free_and_exit2.c\
    src/minishell_execution.c\
    src/minishell_init.c\
    src/minishell_utils.c\
    src/minishell_utils2.c\
    src/minishell_utils3.c\
    src/minishell_utils4.c\
	src/minishell_utils5.c\
	src/minishell_commandfill1.c\
	src/minishell_commandfill2.c\
	src/minishell_piping.c\
	src/minishell_redirection.c\
    src/minishell_purging_quotemarks.c\
	src/minishell_ft_echo.c\
	src/minishell_environment_vars.c\
	src/minishell_ft_export.c\
	src/minishell_ft_env.c\
	src/minishell_ft_unset.c\
	src/minishell_ft_pwd.c\
	src/minishell_ft_cd.c\
	src/minishell_ft_exit.c\
	src/minishell_ft_functions.c\
	src/minishell_commandfill3.c\
	src/minishell_environment_vars2.c\
	src/minishell_piping_builtins.c\
	src/minishell_piping_utils.c\
	src/minishell_piping_utils2.c\
	src/minishell_redirection_utils.c\
	src/minishell_execution_utils.c\

OBJ_DIR = objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

NAME = minishell
LDFLAGS = -lreadline

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re
