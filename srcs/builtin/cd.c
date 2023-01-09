/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 12:59:28 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/09 17:48:13 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *prev_path;

int	cd_chdir(char *cwd, char *path)
{
	int	res;

	res = 0;
	prev_path = cwd;
	res = chdir(path);
	if (res == -1)
	{
		ft_putstr_fd("minsh: cd:", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
	}
	return (res);
}

int	cd_prev(char *temp, char *cwd)
{
	int	res;

	res = 0;
	temp = cwd;
	res = chdir(prev_path);
	prev_path = temp;
	if (res == -1)
	{
		ft_putstr_fd("minsh: cd: OLPWD not set\n", STDERR_FILENO);
		return (-1);
	}
	return (res);
}

int	ft_cd(t_parser_token *parser_token)
{
	char		*path;
	int			res;
	static char	*cwd;
	char		*temp;

	res = 0;
	temp = NULL;
	cwd = getcwd(NULL, 0);
	if (parser_token->cmd->next == NULL)
		return (0);
	if (parser_token->cmd->next->content == NULL)
	{
		prev_path = cwd;
		chdir(getenv("HOME"));
		cwd = getcwd(NULL, 0);
		return (0);
	}
	path = parser_token->cmd->next->content;
	if (ft_strncmp(path, "-", 2) == 0)
		res = cd_prev(temp, cwd);
	else
		cd_chdir(cwd, path);
	return (res);
}
