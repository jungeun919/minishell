/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 13:05:17 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/11 14:31:28 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	put_quote(char *str, int fd)
{
	if (str == NULL || fd < 0)
		return ;
	str++;
	while (*str)
	{
		if (*(str + 1) == '\0')
			break ;
		write(fd, str, STDOUT_FILENO);
		str++;
	}
}
/*
int	check_option(char *content)
{
	int	i;
	int	flag;

	flag = 0;
	if (ft_strncmp(content, "-n", 2) != 0)
		flag = 1;
	i = 2;
	while (content[i])
	{
		if (content[i] == 'n')
			flag++;
		i++;
	}
	printf("flag : %d\ni : %d\n", flag, i);
	if (flag + 2 == i)
		return (1);
	else
		return (0);
}

void	put_echostr(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		if (strncmp(cmd[i], " ", 2) != 0)
		{
			ft_putstr_fd(cmd[i], STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		i++;
	}
}

int	ft_echo(t_exec_token *token)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	if (token->cmd[1] == NULL)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	while (token->cmd[i] != NULL)
	{
		if (check_option(token->cmd[i]))
			option++;
		i++;
	}
	i = option + 1;
	printf("%d\n", i);
	printf("%s =================\n", token->cmd[1]);
	printf("%s =================\n", token->cmd[2]);
	printf("%s =================\n", token->cmd[3]);


	while (token->cmd[i] != NULL)
	{
		//printf("%s -------------\n", token->cmd[i]);
		ft_putstr_fd(token->cmd[i], STDOUT_FILENO);
		if (token->cmd[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (option == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (1);
}
*/

static int	is_nflag(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	if (str[1] != 'n')
		return (0);
	i = 1;
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(char **cmd)
{
	int	i;
	int	nflag;

	nflag = 0;
	i = 1;
	if (cmd[i] != NULL && is_nflag(cmd[i]))
	{
		nflag = 1;
		while (cmd[i] != NULL && is_nflag(cmd[i]))
			i++;
	}
	while (cmd[i] != NULL)
	{
		ft_putstr_fd(cmd[i], STDOUT_FILENO);
		i++;
		if (cmd[i] == NULL)
			break ;
		ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (!nflag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
