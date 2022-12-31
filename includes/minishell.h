
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

typedef struct s_parser_token
{
	t_list *cmd;
	t_list *in;
	t_list *out;
}	t_parser_token;

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
int		check_redirection(t_list *lexer_token);
int		check_double_pipe(t_list *lexer_token);
int		parser_token_size(t_list *lexer_token);
t_parser_token *init_parser_token(int size);
void	make_parser_token(t_list **lexer_token, t_parser_token *parser_token);
void	free_parser_token(t_parser_token *parser_token, int len);
void	sort_redirection(t_parser_token *parser_token, int len);

// builtin
void	ft_env(t_env *list);

// env
t_env	*init_env_list(char **envp);
t_env	*make_env_node(char *key, char *value);
void	env_list_add_node(t_env **list, t_env *node);

char	*get_env_value(t_env *env_list, char *key);

// utils
void	setting_signal(void);
void	sig_handler(int signal);

#endif
