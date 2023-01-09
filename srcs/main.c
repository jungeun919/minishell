/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:28:40 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 13:56:50 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char *cmd;
	int len;
	t_list	*lexer_token;
	t_parser_token	*parser_token;
	t_exec_token	*token;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	g_info.env_list = init_env_list(envp);
	g_info.exit_status = 0;
	while (1)
	{
		set_echoctl_off();
		setting_signal();
		cmd = readline("minishell$ ");
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
		lexer_token = NULL;
		if (parsing_error_handle(parsing(&lexer_token, cmd, g_info.env_list), &lexer_token))
			continue ;
		len = parser_token_size(lexer_token);
		parser_token = init_parser_token(len);
		if (parser_token == NULL)
			clear_and_exit(&lexer_token);
		make_parser_token(&lexer_token, parser_token);
		sort_redirection(parser_token, len);
		token = (t_exec_token *)malloc(sizeof(t_exec_token) * len);
		int	i = 0; // 
		while (i < len)//
		{//
			token[i].parser_token = &(parser_token[i]);//
			token[i].cmd = make_2d_array(parser_token[i].cmd);//
			i++;//
		}//
		exec_cmd(token, g_info.env_list, len);
		// free t_exec_token
		free_parser_token(parser_token, len);
		free(cmd);
	}
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
