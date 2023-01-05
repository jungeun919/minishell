
#include "minishell.h"

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
		temp = ft_split(envp[i], '='); // split 실패시 문제 !
		if (temp == NULL)
		{
			free_env_list(env_list);
			exit(1);
		}
		env_node = make_env_node(temp[0], temp[1]);
		if (env_node == NULL)
		{
			free(temp[0]);
			free(temp[1]);
			free(temp);
			free_env_list(env_list);
			exit(1);
		}
		env_list_add_node(&env_list, env_node);
		// free(temp[0]);
		// free(temp[1]);
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
