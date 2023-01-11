/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:17:57 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/12 03:16:57 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*get_env_path(t_env *env_list, char *key)
{
	t_env	*curr;

	curr = env_list->next->next;
	while (curr != NULL)
	{
		if (!ft_strcmp(curr->key, key))
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

char	*get_cd_path(char *key)
{
	char	*path;

	path = get_env_path(g_info.env_list, key);
	if (!strcmp(key, "HOME"))
		path = getenv(key);
	if (!path)
	{
		printf("minishell: cd: %s not set\n", key);
		return (0);
	}
	return (path);
}

int	exec_chdir(char *path, char *old_path)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("minishell: pwd");
			return (1);
		}
		update_value("PWD", pwd, &g_info.env_list);
		update_value("OLDPWD", old_path, &g_info.env_list);
		return (0);
	}
	else
	{
		printf("minishell: cd: %s No such file or directory\n", path);
		return (1);
	}
}

int	ft_cd(char **cmd, int flag)
{
	char	*old_path;
	char	*path;

	if (cmd[1] == NULL || !ft_strcmp(cmd[1], "~"))
		path = get_cd_path("HOME");
	else if (!ft_strncmp(cmd[1], "~/", 2))
	{
		flag = 1;
		path = ft_strjoin(get_cd_path("HOME"), cmd[1] + 1);
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		path = get_cd_path("OLDPWD");
		if (path)
			printf("%s\n", path);
	}
	else
		path = cmd[1];
	if (!path)
		return (1);
	old_path = getcwd(NULL, 0);
	if (flag == 1)
		free(path);
	return (exec_chdir(path, old_path));
}
