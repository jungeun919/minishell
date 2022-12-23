/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 13:05:31 by sanghan           #+#    #+#             */
/*   Updated: 2022/12/23 13:21:56 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit(t_cmd *cmd)
{
	int	code;

	if (cmd == NULL)
		return (0);
	if (cmd->next == NULL)
	{
		exit(0);
	}
	code = ft_atoi(cmd->next->content);
	exit(code);
}
