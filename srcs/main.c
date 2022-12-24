
#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	printf("%c\n", *argv[0]);

	t_env	*env_list;
	env_list = init_env_list(envp);
	
	/* get command line */
	char *cmd;
	while (1)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
		{
			printf("\033[1A"); // 커서를 위로 한 줄 올리기
			printf("\033[11C"); // 커서를 앞으로 10만큼 전진시키기
			printf("exit");
			exit(0);
		}

		if (ft_strncmp(cmd, "env", 4) == 0)
			ft_env(env_list);

		if (ft_strlen(cmd) >= 1)
			add_history(cmd);
		free(cmd);
		cmd = NULL;
	}
	/* get command line */

	return (0);
}

// builtin-function
void	ft_env(t_env *env_list)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

t_env	*init_env_list(char **envp)
{
	int		i;
	char	**temp;
	t_env	*env_node;
	t_env	*env_list;

	i = 0;
	env_list = NULL; // 아무것도 안들어갔을 때 널 리턴
	while (envp[i] != NULL)
	{
		temp = ft_split(envp[i], '=');
		env_node = make_env_node(temp[0], temp[1]);
		if (!env_node)
		{
			ft_putstr_fd("malloc error\n", STDERR_FILENO);
			exit(1);
		}
		env_list_add_node(&env_list, env_node);
		free(temp);
		temp = NULL;
		i++;
	}
	return (env_list);
}

t_env	*make_env_node(char *key, char *value)
{
	t_env	*env_node;

	env_node = (t_env *)malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = key;
	env_node->value = value;
	env_node->next = NULL;
	return (env_node);
}

void	env_list_add_node(t_env **list, t_env *node)
{
	t_env	*temp;

	if (!node || !list)
		return ;
	if (*list == NULL) // 첫번째 노드
		*list = node;
	else
	{
		temp = *list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
}
