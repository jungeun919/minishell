# include "minishell.h"

static int	is_option(char *line)
{
	if (line && *line == '-' && line + 1)
		return (1);
	return (0);
}

static void	minish_exit(char *msg, int code)
{
	ft_putstr_fd("minish: ", STDERR_FILENO);
	perror(msg);
	exit(code);
}

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
		minish_exit("minish: ft_strdup", 1);
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

void	cd(char **cmds)
{
	int		ret;
	char	*real_path;

	if (is_option(cmds[1]))
	{
		perror_opt(cmds[0], abstract_opt(cmds[1]), "cd [-] [dir]");
		return ;
	}
	real_path = find_real_path(cmds[1]);
	ret = chdir(real_path);
	free(real_path);
	if (ret == -1)
	{
		perror("minishell: cd");

		g_info.exit_status = 2;
		return ;
	}
	set_cd_env();
	g_info.exit_status = 0;
}