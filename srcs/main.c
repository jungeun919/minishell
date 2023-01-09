/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:28:40 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 13:30:05 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	(void)argv;

	t_env	*env_list;
	env_list = init_env_list(envp);

	
	/* get command line */
	char *cmd;

	t_list	*lexer_token;
	// t_list	*temp;
	// int		node_index;
	// char	node_label[20];
	t_parser_token	*parser_token;

	while (1)
	{
		set_echoctl_off();
		setting_signal();

		cmd = readline("minishell$ ");
		// cmd = "echo << $USER";

		if (!cmd) // ctrl + d
		{
			printf("\033[1A"); // 커서를 위로 한 줄 올리기
			printf("\033[11C"); // 커서를 앞으로 11만큼 전진시키기
			printf("exit");
			exit(0);
		}

		if (!(*cmd)) // blank
		{
			free(cmd);
			continue ;
		}

		if (ft_strlen(cmd) >= 1)
			add_history(cmd);

		/* lexer main */
		// lexer_token = NULL;
		// temp = lexer_token;
		// node_index = 0;
		// (void)node_index;
		// (void)temp;

		// lexer(cmd, &lexer_token);
		// labeling(lexer_token);
		// labeling_after_heredoc(lexer_token);
		// if (check_odd_quote(lexer_token))
		// {
		// 	ft_lstclear(&lexer_token, free);
		// 	printf("odd quote is not allowed!\n");
		// 	continue ;
		// }

		// //	parsser
		// replace_env(lexer_token, env_list);
		// remove_quote(&lexer_token);
		// merge_string(&lexer_token);
		// delete_blank(&lexer_token);
		// if (check_redirection(lexer_token) || check_double_pipe(lexer_token))
		// {
		// 	printf("no string after redirection! or double pipe\n");
		// 	ft_lstclear(&lexer_token, free);
		// 	continue ;
		// }
		lexer_token = NULL;
		if (parsing_error_handle(parsing(&lexer_token, cmd, env_list), &lexer_token))
			continue ;

		/*
		temp = lexer_token;
		node_index = 0;
		while (temp != NULL) // for test
		{
			if		(temp->label == 0) ft_strlcpy( node_label , "NORMAL_STRING",20);
			else if (temp->label == 1) ft_strlcpy( node_label , "DOUBLE_QUOTE",20);
			else if (temp->label == 2) ft_strlcpy( node_label , "SINGLE_QUOTE",20);
			else if (temp->label == 3) ft_strlcpy( node_label , "BLANK",20);
			else if (temp->label == 4) ft_strlcpy( node_label , "PIPE",20);
			else if (temp->label == 5) ft_strlcpy( node_label , "REDIR_IN",20);
			else if (temp->label == 6) ft_strlcpy( node_label , "REDIR_HEREDOC",20);
			else if (temp->label == 7) ft_strlcpy( node_label , "REDIR_OUT",20);
			else if (temp->label == 8) ft_strlcpy( node_label , "REDIR_DOUBLE_OUT",20);
			else if (temp->label == 9) ft_strlcpy( node_label , "AFTER_HEREDOC",20);
			else if (temp->label == 10) ft_strlcpy( node_label , "AFTER_HEREDOC_DQ",20);
			else if (temp->label == 11) ft_strlcpy( node_label , "AFTER_HEREDOC_SQ",20);
			
			printf("%2d 번째 노드 : [%s] <- (%s)\n", node_index++, temp->content, node_label);
			temp = temp->next;
		} // for test
		*/

		int len = parser_token_size(lexer_token);
		// printf("len : %d\n", len);
		parser_token = init_parser_token(len);
		if (parser_token == NULL)
			clear_and_exit(&lexer_token);// 에러처리 필요
		make_parser_token(&lexer_token, parser_token);
		sort_redirection(parser_token, len);

		/*
		for (int i = 0; i < len; i++){ // for test
			printf("%d 번째 파서 토큰 ------------------------ \n", i);
			printf("cmd : ");
			temp = parser_token[i].cmd; // cmd
			while (temp != NULL)
			{
				printf("[%s]->", temp->content);
				temp = temp->next;
			}
			printf("(null)\n");
			printf("in : ");
			temp = parser_token[i].in; // in
			while (temp != NULL)
			{
				printf("[%s]->", temp->content);
				temp = temp->next;
			}
			printf("(null)\n");
			printf("out : ");
			temp = parser_token[i].out; // out
			while (temp != NULL)
			{
				printf("[%s]->", temp->content);
				temp = temp->next;
			}
			printf("(null)\n");
		} // for test
		*/

		// free_parser_token(parser_token, len);

		t_exec_token	*token;

		token = (t_exec_token *)malloc(sizeof(t_exec_token) * len);

		int	i = 0;
		while (i < len)
		{
			token[i].parser_token = &(parser_token[i]);
			token[i].cmd = make_2d_array(parser_token[i].cmd);
			i++;
		}

		exec_cmd(token, env_list, len);
		free_parser_token(parser_token, len);
		free(cmd);
		// system("leaks minishell");
	}
	/* get command line */

	return (0);
}

char	**make_2d_array(t_list *cmd_list)
{
	int		i;
	char	**cmd;
	t_list	*temp;

	cmd = (char **)malloc(sizeof(char *) * (ft_lstsize(cmd_list) + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	temp = cmd_list;
	while (temp != NULL)
	{
		cmd[i] = temp->content;
		i++;
		temp = temp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
