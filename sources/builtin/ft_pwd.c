/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 18:38:57 by sanghan           #+#    #+#             */
/*   Updated: 2022/12/23 15:52:59 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(char *cmd)
{
	char	*path;

	if (cmd == NULL)
		return (0);
	path = getcwd(NULL, 0);
	if (!path)
	{
		ft_putstr_fd("Error Path\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(path);
	return (0);
}
