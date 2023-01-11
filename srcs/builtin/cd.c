/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:17:57 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/11 20:28:37 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	abstract_opt(char *line)
{
	line++;
	while (*line && *(line + 1))
	{
		if (*line != *(line + 1))
			return (0);
		line++;
	}
	return (*line);
}

static void	perror_opt(char *cmd, char opt, char *usage)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": -", STDERR_FILENO);
	ft_putchar_fd(opt, STDERR_FILENO);
	ft_putendl_fd(": invalid option", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": usage: ", STDERR_FILENO);
	ft_putendl_fd(usage, STDERR_FILENO);
	g_info.exit_status = 2;
}

static char	*find_real_path(char *path)
{
	char	*home;
	char	*real_path;

	if (!path)
	{
		home = get_env_value(g_info.env_list, "HOME");
		if (!home)
			real_path = ft_strdup("");
		else
			real_path = ft_strdup(home);
	}
	else
		real_path = ft_strdup(path);
	if (real_path == 0)
	{
		ft_putstr_fd("minish: ", STDERR_FILENO);
		perror("minish: ft_strdup");
		exit(1);
	}
	return (real_path);
}

static void	set_cd_env(void)
{
	char	*oldpwd;
	char	*pwd;
	char	*real_path;
	t_env	*node;

	oldpwd = get_env_value(g_info.env_list, "OLDPWD");
	pwd = get_env_value(g_info.env_list, "PWD");
	if (oldpwd)
		delete_node("OLDPWD", &g_info.env_list);
	node = make_env_node(ft_strdup("OLDPWD"), ft_strdup(pwd));
	if (!node)
		return ;
	env_list_add_node(&g_info.env_list, node);
	if (pwd)
		delete_node("PWD", &g_info.env_list);
	real_path = getcwd(NULL, 0);
	if (!real_path)
	{
		perror("minishell: cd: ");
		g_info.exit_status = 2;
		return ;
	}
	node = make_env_node(ft_strdup("PWD"), ft_strdup(real_path));
	if (!node)
		return ;
	env_list_add_node(&g_info.env_list, node);
}

int	ft_cd(char **cmd)
{
	int		ret;
	char	*real_path;

	if (cmd[1] && *cmd[1] == '-' && cmd[1] + 1)
	{
		perror_opt(cmd[0], abstract_opt(cmd[1]), "cd [-] [dir]");
		return (0);
	}
	real_path = find_real_path(cmd[1]);
	ret = chdir(real_path);
	free(real_path);
	if (ret == -1)
	{
		perror("minishell: cd");
		return (2);
	}
	set_cd_env();
	return (0);
}
