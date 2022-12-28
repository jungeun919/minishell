/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungchoi <jungchoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:17:25 by jungchoi          #+#    #+#             */
/*   Updated: 2022/12/28 14:25:04 by jungchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_list *lexer_token)
{
	char	*cmd;

	cmd = lexer_token->content;
	if (ft_strncmp(cmd, "env", 4) == 0 || \
		ft_strncmp(cmd, "export", 7) == 0 || \
		ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else
		return (0);
}

void	exec_builtin(t_list *lexer_token, t_env *env_list)
{
	char	*cmd;

	cmd = lexer_token->content;
	if (ft_strncmp(cmd, "env", 4) == 0)
	{
		t_execute_unit	*cmd;
		cmd = ft_calloc(0, sizeof(t_execute_unit));

		cmd->command = "env";
		cmd->next = NULL;

		ft_env(cmd, env_list);
	}

	else if (ft_strncmp(cmd, "export", 7) == 0)
	{
		t_execute_unit	*cmd;
		t_execute_unit	*cmd2;
		cmd = ft_calloc(0, sizeof(t_execute_unit));
		cmd2 = ft_calloc(0, sizeof(t_execute_unit));

		// invalid key test
		cmd->command = "export";
		cmd->next = cmd2;
		cmd2->command = "+testkey=testvalue";
		cmd2->next = NULL;

		ft_export(cmd, env_list);
		print_env_list(env_list);
		printf("\n\n\n\n");

		// add node test
		cmd->command = "export";
		cmd->next = cmd2;
		cmd2->command = "_testkey=testvalue";
		cmd2->next = NULL;

		ft_export(cmd, env_list);
		print_env_list(env_list);
		printf("\n\n\n\n");

		// update value test
		cmd->command = "export";
		cmd->next = cmd2;
		cmd2->command = "_testkey=updatevalue";
		cmd2->next = NULL;

		ft_export(cmd, env_list);
		print_env_list(env_list);
	}

	else if (ft_strncmp(cmd, "unset", 6) == 0)
	{
		t_execute_unit	*cmd;
		t_execute_unit	*cmd2;
		t_execute_unit	*cmd3;
		t_execute_unit	*cmd4;
		t_execute_unit	*cmd5;
		t_execute_unit	*cmd6;
		cmd = ft_calloc(0, sizeof(t_execute_unit));
		cmd2 = ft_calloc(0, sizeof(t_execute_unit));
		cmd3 = ft_calloc(0, sizeof(t_execute_unit));
		cmd4 = ft_calloc(0, sizeof(t_execute_unit));
		cmd5 = ft_calloc(0, sizeof(t_execute_unit));
		cmd6 = ft_calloc(0, sizeof(t_execute_unit));

		// add node1
		cmd->command = "export";
		cmd->next = cmd2;
		cmd2->command = "_testkey1=testvalue1";
		cmd2->next = NULL;
		ft_export(cmd, env_list);

		// add node 2
		cmd3->command = "export";
		cmd3->next = cmd4;
		cmd4->command = "_testkey2=testvalue2";
		cmd4->next = NULL;
		ft_export(cmd3, env_list);

		// delete node test
		cmd5->command = "unset";
		cmd5->next = cmd6;
		cmd6->command = "_testkey1";
		cmd6->next = NULL;
		ft_unset(cmd5, env_list);
		print_env_list(env_list);
	}
}

