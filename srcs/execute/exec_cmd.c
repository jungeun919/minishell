
#include "minishell.h"

static void	rm_all_heredoc_file()
{
	int		i;
	char	*filename;

	i = 0;
	while (i < g_info.heredoc_cnt)
	{
		filename = ft_join_and_free("/tmp/", ft_itoa(i));
		unlink(filename);
		free(filename);
		i++;
	}
}

void	exec_cmd(t_exec_token *token, t_env *env_list, int len)
{
	pid_t	*pids;
	int		**fds;
	int		i;

	if (len == 1 && is_builtin(token))
		return exec_builtin(token, env_list);
	set_heredoc_input(token, env_list, len);
	if (token->parser_token->cmd == NULL) // hi
		return ;
	init_exec_info(&pids, &fds, len);
	i = 0;
	while (i < len)
	{
		exec_pipe(token[i], i, pids, fds, env_list, len);
		i++;
	}
	close_all_fds(fds, len);
	wait_all_childs(pids, len);
	free_init_exec_info(&pids, &fds, len - 1);
	rm_all_heredoc_file();
	return ;
}

void	run_execve_cmd(char **cmd_list, t_env *env_list)
{
	char	*cmd;
	char	*path;
	char	**env;
	
	if (!cmd_list)
		return ;
	env = convert_env_list_to_str_list(env_list);
	if (ft_strchr(cmd_list[0], '/'))
		path = cmd_list[0];
	else
	{
		cmd = ft_strjoin("/", cmd_list[0]);
		path = get_path(cmd, env);
		free(cmd);
	}
	if (path)
	{
		if (execve(path, cmd_list, env) == -1)
			error_exit("command not found\n", 127);
	}
	else
	{
		free_2d_array(env);
		free(path);
		error_exit("command not found\n", 127);
	}
}

char	**convert_env_list_to_str_list(t_env *env_list)
{
	int		i;
	char	**env_str;
	t_env	*temp;

	temp = env_list;
	i = 0;
	while (temp)
	{
		if (temp->value != NULL)
			i++;
		temp = temp->next;
	}
	env_str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env_str)
		return (NULL);
	join_key_and_value(env_str, env_list);
	return (env_str);
}

void	join_key_and_value(char **env_str, t_env *env_list)
{
	int		i;
	t_env	*temp;
	char	*join_key;
	char	*join_all;

	temp = env_list;
	i = 0;
	while (temp)
	{
		if (temp->value != NULL)
		{
			join_key = ft_strjoin(temp->key, "=");
			if (join_key == NULL)
				error_exit("malloc error\n", 1);
			join_all = ft_strjoin(join_key, temp->value);
			free(join_key);
			env_str[i] = join_all;
			if (join_all == NULL)
				error_exit("malloc error\n", 1);
			i++;
		}
		temp = temp->next;
	}
	env_str[i] = NULL;
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**split_path;
	char	*path;

	if (!cmd)
		return (NULL);
	i = 0;
	while (env[i] && (ft_strncmp("PATH", env[i], 4) != 0))
		i++;
	split_path = ft_split(env[i] + 5, ':');
	i = 0;
	while (split_path[i])
	{
		path = ft_strjoin(split_path[i], cmd);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
		{
			free_2d_array(split_path);
			return (path);
		}
		free(path);
		i++;
	}
	free_2d_array(split_path);
	return (NULL);
}
