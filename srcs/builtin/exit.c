/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 13:05:31 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/09 16:01:51 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
/*
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
}
*/
