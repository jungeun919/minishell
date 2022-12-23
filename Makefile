NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INCLUDES_DIR = includes
LIBS_DIR = libs
SRCS_DIR = srcs
OBJS_DIR = objs

# readline
LDFLAGS = -L/opt/homebrew/opt/readline/lib
CPPFLAGS = -I/opt/homebrew/opt/readline/include
# LDFLAGS = -L/goinfre/jungchoi/.brew/opt/readline/lib
# CPPFLAGS = -I/goinfre/jungchoi/.brew/opt/readline/include
READ_LIB = -lreadline

LIBFT_DIR = libs/libft
LIBFT = libft.a
LIBFT_LIB = -lft

SRCS = $(addprefix $(SRCS_DIR)/, \
	main.c)

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

all : $(NAME)

$(NAME) : $(LIBFT) $(OBJS)
	make -C $(LIBFT_DIR) all
	$(CC) $(CFLAGS) $(OBJS) -I $(INCLUDES_DIR) $(LDFLAGS) $(CPPFLAGS) $(READ_LIB) -L $(LIBFT_DIR) $(LIBFT_LIB) -o $@

$(LIBFT) :
	make -C $(LIBFT_DIR) all
	
$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	@mkdir $(OBJS_DIR) 2> /dev/null || true
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -I $(LIBFT_DIR) -c $< -o $@

clean :
	rm -rf $(OBJS_DIR)
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