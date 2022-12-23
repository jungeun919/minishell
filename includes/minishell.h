/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:51:04 by sanghan           #+#    #+#             */
/*   Updated: 2022/12/23 15:56:21 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "../libft/get_next_line.h"

# define MAXSIZE		1024

# define FALSE			0
# define TRUE			1

# define ERROR			-1
# define SUCCESS 		1

# define STDIN 			0
# define STDOUT 		1
# define STDERR 		2

# define COLOR_BR_BLUE	"\033[34;1m"
# define COLOR_WHITE	"\033[37m"
# define COLOR_YELLOW	"\033[33m"
# define COLOR_CYAN		"\033[36m"
# define END_COLOR		"\033[0m"

typedef struct s_cmd
{
	struct s_cmd	*prev;
	char			*content;
	struct s_cmd	*next;
}	t_cmd;

#endif
