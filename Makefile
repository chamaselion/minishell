CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC = src/loop/minishell_loop.c\
 	src/loop/minishell_loop_utils.c\
    src/tokenizer/minishell_tokenizer2.c\
    src/tokenizer/minishell_tokenizer3.c\
    src/tokenizer/minishell_tokenizer.c\
	src/command/minishell_concatenation.c\
    src/environment/minishell_variable_expansion.c\
	src/environment/minishell_variable_expansion2.c\
    src/roles/minishell_roles_cmdexpected.c\
	src/roles/minishell_roles_cmdexpected2.c\
    src/syntax/minishell_syntax.c\
    src/signal/minishell_signal.c\
    src/quote/minishell_quote_handling.c\
	src/quote/minishell_quote_handling2.c\
    src/free/minishell_free_and_exit.c\
    src/free/minishell_free_and_exit2.c\
    src/execution/minishell_execution.c\
    src/init/minishell_init.c\
    src/utils/minishell_utils.c\
    src/utils/minishell_utils2.c\
    src/utils/minishell_utils3.c\
    src/utils/minishell_utils4.c\
	src/utils/minishell_utils5.c\
	src/command/minishell_commandfill1.c\
	src/command/minishell_commandfill2.c\
	src/piping/minishell_piping.c\
	src/redirection/minishell_redirection.c\
    src/quote/minishell_purging_quotemarks.c\
	src/command/minishell_ft_echo.c\
	src/environment/minishell_environment_vars.c\
	src/command/minishell_ft_export.c\
	src/command/minishell_ft_env.c\
	src/command/minishell_ft_unset.c\
	src/command/minishell_ft_pwd.c\
	src/command/minishell_ft_cd.c\
	src/command/minishell_ft_exit.c\
	src/command/minishell_ft_functions.c\
	src/command/minishell_commandfill3.c\
	src/environment/minishell_environment_vars2.c\
	src/piping/minishell_piping_builtins.c\
	src/piping/minishell_piping_utils.c\
	src/piping/minishell_piping_utils2.c\
	src/redirection/minishell_redirection_utils.c\
	src/execution/minishell_execution_utils.c\

OBJ_DIR = objects
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

NAME = minishell
LDFLAGS = -lreadline

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
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
