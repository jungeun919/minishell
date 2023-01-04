
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INCLUDES_DIR = includes
LIBS_DIR = libs
SRCS_DIR = srcs

# srcs directory
LEXER_DIR = lexer
PARSER_DIR = parser
BUILTIN_DIR = builtin
EXEC_DIR = execute
ENV_DIR = env
UTILS_DIR = utils

LEXER_SRCS = lexer.c labeling.c check_odd_quote.c lexer_error.c
PARSER_SRCS = check.c delete_blank.c merge_string.c parser_token.c remove_quote.c replace_env.c sort_redirection.c
BUILTIN_SRCS = env.c export.c unset.c
EXEC_SRCS = exec_builtin.c exec_cmd.c redir.c exec_pipe.c
ENV_SRCS = init_env.c utils.c
UTILS_SRCS = signal.c

# readline
# LDFLAGS = -L/opt/homebrew/opt/readline/lib
# CPPFLAGS = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/goinfre/$(USER)/.brew/opt/readline/lib
CPPFLAGS = -I/goinfre/$(USER)/.brew/opt/readline/include
READ_LIB = -lreadline

LIBFT_DIR = libs/libft
LIBFT = libft.a
LIBFT_LIB = -lft

SRCS = $(addprefix $(SRCS_DIR)/, main.c) \
	$(addprefix $(SRCS_DIR)/$(LEXER_DIR)/, $(LEXER_SRCS)) \
	$(addprefix $(SRCS_DIR)/$(PARSER_DIR)/, $(PARSER_SRCS)) \
	$(addprefix $(SRCS_DIR)/$(BUILTIN_DIR)/, $(BUILTIN_SRCS)) \
	$(addprefix $(SRCS_DIR)/$(EXEC_DIR)/, $(EXEC_SRCS)) \
	$(addprefix $(SRCS_DIR)/$(ENV_DIR)/, $(ENV_SRCS)) \
	$(addprefix $(SRCS_DIR)/$(UTILS_DIR)/, $(UTILS_SRCS))

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	make -C $(LIBFT_DIR) bonus
	$(CC) $(CFLAGS) $(OBJS) -I $(INCLUDES_DIR) $(READ_LIB) $(LDFLAGS) $(CPPFLAGS) -L $(LIBFT_DIR) $(LIBFT_LIB) -o $@
	
%.o : %.c
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) $(CPPFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean : clean
	rm -rf $(NAME)
	make -C $(LIBFT_DIR) fclean

re :
	make fclean
	make all

.PHONY = all clean fclean re

# ==========lexer==========
# NAME = minishell

# CC = cc
# CFLAGS = -Wall -Wextra -Werror -g
# # LDFLAGS = -L/opt/homebrew/opt/readline/lib
# # CPPFLAGS = -I/opt/homebrew/opt/readline/include
# READ_LIB =  -lreadline
# RM = rm -rf

# LIB_PATH = ./libs/libft
# SRCS_PATH = ./srcs/lexer
# OBJS_PATH = ./objects
# INCLUDES = ./includes


# SRCS = lexer.c
# OBJS_NAME = $(SRCS:.c=.o)
# OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAME))

# $(OBJS_PATH)/%.o: $(SRCS_PATH)/%.c
# 	mkdir -p $(OBJS_PATH)
# 	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# $(NAME) : $(OBJS)
# 	make -C $(LIB_PATH) bonus
# 	$(CC) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) $(READ_LIB) -L$(LIB_PATH) -lft $^ -o $@

# all: $(NAME)

# clean:
# 	make -C $(LIB_PATH) clean
# 	$(RM) $(OBJS_PATH)

# fclean: clean
# 	make -C $(LIB_PATH) fclean
# 	$(RM) $(NAME)

# re:
# 	$(MAKE) fclean
# 	$(MAKE) all

# .PHONY: all bonus clean fclean re
