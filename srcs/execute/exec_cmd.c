
#include "minishell.h"

void	exec_cmd(t_list *lexer_token, t_env *env_list)
{
	t_execute_unit	*token;
	token = ft_calloc(0, sizeof(t_execute_unit));
	token->command = (char **)malloc(999);
	token->command[0] = (char *)malloc(999);
	token->command[1] = (char *)malloc(999);
	token->command[2] = (char *)malloc(999);

	token->command[0] = "cat";
	token->command[1] = "a.txt";
	token->command[2] = NULL;

	// token->command[0] = "ls";
	// token->command[1] = "-al";
	// token->command[2] = NULL;

	lexer_token->content = "";

	char    **cmd_list;
	cmd_list = convert_token_to_str_list(token);
	run_execve_cmd(cmd_list, env_list);
}

char	**convert_token_to_str_list(t_execute_unit *token)
{
	int		i;
	char	**cmd_str;

	if (token == NULL)
		return (NULL);
	i = 0;
	while (token->command[i] != NULL)
		i++;
	cmd_str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cmd_str)
		return (NULL);
	i = 0;
	while (token->command[i] != NULL)
	{
		cmd_str[i] = ft_strdup(token->command[i]);
		i++;
	}
	cmd_str[i] = NULL;
	return (cmd_str);
}

void	run_execve_cmd(char **cmd_list, t_env *env_list)
{
	char	*cmd;
	char	*path;

	char	**env;
	env = convert_env_list_to_str_list(env_list);
	
	cmd = ft_strjoin("/", cmd_list[0]);
	path = get_path(cmd, env);
	if (!path)
		error_exit("path error\n", 1);
	if (execve(path, cmd_list, env) == -1)
		error_exit("execve error\n", 127);
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
			free_split(split_path);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(split_path);
	return (NULL);
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	error_exit(char *str, int status)
{
	ft_putstr_fd(str, 2);
	exit(status);
}
