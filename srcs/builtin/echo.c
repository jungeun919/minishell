/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 13:05:17 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/10 18:19:16 by sanghan          ###   ########.fr       */
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

int	check_option(char *content)
{
	int	i;
	int flag;

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
//	printf("flag : %d\n", flag);
//	printf("i : %d\n", i);
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
	//int	flag;

	option = 0;
	//flag = 0;
	if (token->cmd[1] == NULL)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	if (token->cmd[1])
		option = check_option(token->cmd[1]);
	if (option)
		i = 2;
	else
		i = 1;
	while (token->cmd[i] != NULL)
	{
		ft_putstr_fd(token->cmd[i], STDOUT_FILENO);
		//put_echostr(&(token->cmd[i]));
		if (token->cmd[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (!option)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (1);
}
