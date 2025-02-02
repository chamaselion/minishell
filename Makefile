CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC = minishell_loop.c\
    minishell_tokenizer2.c\
    minishell_tokenizer3.c\
    minishell_tokenizer.c\
	minishell_concatenation.c\
    minishell_variable_expansion.c\
	minishell_variable_expansion2.c\
    minishell_roles_cmdexpected.c\
	minishell_roles_cmdexpected2.c\
    minishell_syntax.c\
    minishell_signal.c\
    minishell_quote_handling.c\
	minishell_quote_handling2.c\
    minishell_free_and_exit.c\
    minishell_free_and_exit2.c\
    minishell_execution.c\
    minishell_init.c\
    minishell_utils.c\
    minishell_utils2.c\
    minishell_utils3.c\
    minishell_utils4.c\
	minishell_utils5.c\
	minishell_commandfill1.c\
	minishell_commandfill2.c\
	minishell_piping.c\
	minishell_redirection.c\
    minishell_purging_quotemarks.c\
	minishell_ft_echo.c\
	minishell_environment_vars.c\
	minishell_ft_export.c\
	minishell_ft_env.c\
	minishell_ft_unset.c\
	minishell_ft_pwd.c\
	minishell_ft_cd.c\
	minishell_ft_exit.c\
	minishell_ft_functions.c\
	minishell_commandfill3.c\
	minishell_environment_vars2.c\
	minishell_piping_builtins.c\
	minishell_piping_utils.c\
	minishell_piping_utils2.c\
	minishell_redirection_utils.c\
	minishell_execution_utils.c\
	
OBJ_DIR = objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

NAME = minishell
LDFLAGS = -lreadline

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -I. -c $< -o $@

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
