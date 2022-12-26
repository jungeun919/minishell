
#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	printf("%c\n", *argv[0]);

	t_env	*env_list;
	env_list = init_env_list(envp);

	/* set ECHOCTL on/off */
	struct termios	term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL); // 제어문자 반향
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // 변경 속성 바로 적용
	/* set ECHOCTL on/off */

	setting_signal();
	
	/* get command line */
	char *cmd;

	t_list	*lexer_token;
	t_list	*temp;
	int		node_index;
	char	node_label[20];

	while (1)
	{
		cmd = readline("minishell$ ");

		if (!cmd) // ctrl + d
		{
			printf("\033[1A"); // 커서를 위로 한 줄 올리기
			printf("\033[11C"); // 커서를 앞으로 11만큼 전진시키기
			printf("exit");
			exit(0);
		}

		/* lexer main */
		lexer_token = NULL;
		temp = lexer_token;
		node_index = 0;

		lexer(cmd, &lexer_token);
		labeling(lexer_token);
		if (check_odd_quote(lexer_token))
		{
			ft_lstclear(&lexer_token, free);
			printf("odd quote is not allowed!\n");
			continue ;
		}

		//	parsser
		replace_env(lexer_token, env_list);
		remove_quote(&lexer_token);
		merge_string(&lexer_token);
		delete_blank(&lexer_token);

		temp = lexer_token;
		node_index = 0;
		while (temp != NULL)
		{
			if		(temp->label == 0) ft_strlcpy( node_label , "NOMAL_STRING",20);
			else if (temp->label == 1) ft_strlcpy( node_label , "DOUBLE_QUOTE",20);
			else if (temp->label == 2) ft_strlcpy( node_label , "SINGLE_QUOTE",20);
			else if (temp->label == 3) ft_strlcpy( node_label , "BLANK",20);
			else if (temp->label == 4) ft_strlcpy( node_label , "PIPE",20);
			else if (temp->label == 5) ft_strlcpy( node_label , "REDIR_IN",20);
			else if (temp->label == 6) ft_strlcpy( node_label , "REDIR_HEREDOC",20);
			else if (temp->label == 7) ft_strlcpy( node_label , "REDIR_OUT",20);
			else if (temp->label == 8) ft_strlcpy( node_label , "REDIR_DOUBLE_OUT",20);
			
			printf("%2d 번째 노드 : [%s] <- (%s)\n", node_index++, temp->content, node_label);
			temp = temp->next;
		}

		ft_lstclear(&lexer_token, free);
		/* lexer main */


		if (ft_strncmp(cmd, "env", 4) == 0)
			ft_env(env_list);

		if (ft_strlen(cmd) >= 1)
			add_history(cmd);
		free(cmd);
		cmd = NULL;
	}
	/* get command line */

	return (0);
}
