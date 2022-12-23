# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/15 19:17:06 by sanghan           #+#    #+#              #
#    Updated: 2022/12/22 19:01:11 by sanghan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

LIB_PATH = ./libft
SRCS_PATH = ./sources
OBJS_PATH = ./objects
INCLUDES = ./includes

#LDFLAGS = -L/opt/homebrew/opt/readline/lib
#CPPFLAGS = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/goinfre/sanghan/.brew/opt/readline/lib
CPPFLAGS = -I/goinfre/sanghan/.brew/opt/readline/include
READ_LIB = -lreadline

SRCS = main.c
OBJS_NAME = $(SRCS:.c=.o)
OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAME))

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.c
	mkdir -p $(OBJS_PATH)
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

$(NAME) : $(OBJS)
	make -C $(LIB_PATH) all
	$(CC) $(CFLAGS) -I $(INCLUDES) $(LDFLAGS) $(CPPFLAGS) $(READ_LIB) -L$(LIB_PATH) -lft $^ -o $@

all: $(NAME)

clean:
	make -C $(LIB_PATH) clean
	$(RM) $(OBJS_PATH)

fclean: clean
	make -C $(LIB_PATH) fclean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all bonus clean fclean re
