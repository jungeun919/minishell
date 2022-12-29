
#include "minishell.h"

int	ft_unset(t_execute_unit *argv, t_env *env_list)
{
	int	i;

	if (argv->command[1] == NULL) // not enough arguments
		return (0);
	i = 1;
	while (argv->command[i])
	{
		if (is_key_in_env_list(argv->command[i], env_list))
			delete_node(argv->command[i], &env_list);
		i++;
	}
	return (1);
}

void	delete_node(char *key, t_env **env_list)
{
	t_env	*temp;
	t_env	*del_node;

	temp = *env_list;
	del_node = get_node(key, *env_list);
	if (!del_node)
		return ;
	while (temp)
	{
		if (temp->next == del_node)
		{
			temp->next = del_node->next;
			free(del_node->key);
			free(del_node->value);
			free(del_node);
		}
		else
			temp = temp->next;
	}
}

t_env	*get_node(char *key, t_env *env_list)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
