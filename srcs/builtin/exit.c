/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 13:05:31 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/10 16:41:23 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
int	ft_exit(t_parser_token *parser_token)
{
	int	code;

	if (parser_token->cmd == NULL)
		return (0);
	if (parser_token->cmd->next == NULL)
	{
		exit(0);
	}
	code = ft_atoi(parser_token->cmd->next->content);
	exit(code);
}

int	ft_exit(char **cmd)
{
	int	i;
	int	code;

	i = 1;
	if (cmd[1] == NULL)
		return (0);
	while (cmd[i])
	{
		code = ft_atoi(cmd[i]);
		exit(code);
	}
	exit(0);
}*/

int		ft_isdigit_str(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (!ft_isdigit(str[idx]))
			return (0);
		idx++;
	}
	return (1);
}

void	ft_exit_errcode(void)
{
	ft_putstr_fd("exit\n", STDERR);
	exit(g_info.exit_status);
}

int	ft_exit(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
		i++;
	if (i == 1)
		ft_exit_errcode();
	else if (i == 2 && ft_isdigit_str(cmd[1]))
		g_info.exit_status = ft_atoi(cmd[1]);
	else if (i > 2 && ft_isdigit_str(cmd[1]))
	{
		ft_putstr_fd("exit\n", STDERR);
		printf("exit : too many arguments\n");
		g_info.exit_status = 1;
		return (0);
	}
	else
	{
		printf("exit : numeric argument required\n");
		g_info.exit_status = 255;
		exit(g_info.exit_status & 255);
	}
	ft_exit_errcode();
	return (0);
}
