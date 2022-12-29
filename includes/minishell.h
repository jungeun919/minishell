
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libs/libft/libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;



/* builtin test */
typedef struct s_execute_unit
{
	char					**command;
	// char					*type;
}	t_execute_unit;

int		is_builtin(t_list *lexer_token);
void	exec_builtin(t_list *lexer_token, t_env *env_list);

// env
int		ft_env(t_execute_unit *argv, t_env *env_list);
void	print_env_list(t_env *env_list);

// export
int		ft_export(t_execute_unit *argv, t_env *env_list);
void	execute_export(char *key, char *value, t_env *env_list);
int		is_key_in_env_list(char *key, t_env *env_list);
int		is_valid_format_key(char *key);
void	update_value(char *key, char *value, t_env **env_list);

// unset
int		ft_unset(t_execute_unit *argv, t_env *env_list);
void	delete_node(char *key, t_env **env_list);
t_env	*get_node(char *key, t_env *env_list);
/* builtin test */



// lexer
void	free_temp_clear_and_exit(t_list **lexer_token, char *temp);
void	clear_and_exit(t_list **lexer_token);
void	lexer(char *str, t_list **lexer_token);
void	labeling(t_list *lexer_token);
void	labeling_after_heredoc(t_list *lexer_token);
int		check_odd_quote(t_list *lexer_token);
void	replace_env(t_list *lexer_token, t_env *env_list);
void	remove_quote(t_list **lexer_token);
void	merge_string(t_list **lexer_token);
void	delete_blank(t_list **lexer_token);

// env
t_env	*init_env_list(char **envp);
t_env	*make_env_node(char *key, char *value);
void	env_list_add_node(t_env **list, t_env *node);

char	*get_env_value(t_env *env_list, char *key);

// utils
void	setting_signal(void);
void	sig_handler(int signal);

#endif
