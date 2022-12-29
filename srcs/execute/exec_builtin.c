/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungeun <jungeun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:17:25 by jungchoi          #+#    #+#             */
/*   Updated: 2022/12/29 16:14:28 by jungeun          ###   ########.fr       */
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
		t_execute_unit	*temp;
		temp = malloc(sizeof(t_execute_unit));
		temp->command = (char **)malloc(999);
		temp->command[0] = (char *)malloc(999);
		temp->command[1] = (char *)malloc(999);

		temp->command[0] = "env";
		temp->command[1] = NULL;
		
		ft_env(temp, env_list);
	}

	else if (ft_strncmp(cmd, "export", 7) == 0)
	{
		t_execute_unit	*temp;
		temp = malloc(sizeof(t_execute_unit));
		temp->command = (char **)malloc(999);
		temp->command[0] = (char *)malloc(999);
		temp->command[1] = (char *)malloc(999);
		temp->command[2] = (char *)malloc(999);

		// invalid key test
		temp->command[0] = "export";
		temp->command[1] = "+testkey=testvalue";
		temp->command[2] = NULL;

		ft_export(temp, env_list);
		print_env_list(env_list);
		printf("\n\n\n\n");

		// add node test
		temp->command[0] = "export";
		temp->command[1] = "_testkey=testvalue";
		temp->command[2] = NULL;

		ft_export(temp, env_list);
		print_env_list(env_list);
		printf("\n\n\n\n");

		// update value test
		temp->command[0] = "export";
		temp->command[1] = "_testkey=updatevalue";
		temp->command[2] = NULL;

		ft_export(temp, env_list);
		print_env_list(env_list);
	}

	else if (ft_strncmp(cmd, "unset", 6) == 0)
	{
		t_execute_unit	*temp;
		t_execute_unit	*temp2;
		t_execute_unit	*temp3;
		temp = malloc(sizeof(t_execute_unit));
		temp2 = malloc(sizeof(t_execute_unit));
		temp3 = malloc(sizeof(t_execute_unit));
		temp->command = (char **)malloc(999);
		temp->command[0] = (char *)malloc(999);
		temp->command[1] = (char *)malloc(999);
		temp->command[2] = (char *)malloc(999);

		temp2->command = (char **)malloc(999);
		temp2->command[0] = (char *)malloc(999);
		temp2->command[1] = (char *)malloc(999);
		temp2->command[2] = (char *)malloc(999);

		temp3->command = (char **)malloc(999);
		temp3->command[0] = (char *)malloc(999);
		temp3->command[1] = (char *)malloc(999);
		temp3->command[2] = (char *)malloc(999);

		// add node1
		temp->command[0] = "export";
		temp->command[1] = "_testkey1=testvalue1";
		temp->command[2] = NULL;
		ft_export(temp, env_list);

		// add node 2
		temp2->command[0] = "export";
		temp2->command[1] = "_testkey2=testvalue2";
		temp2->command[2] = NULL;
		ft_export(temp2, env_list);

		// delete node test
		temp3->command[0] = "unset";
		temp3->command[1] = "_testkey1";
		temp3->command[2] = NULL;
		ft_unset(temp3, env_list);
		print_env_list(env_list);
	}
}

