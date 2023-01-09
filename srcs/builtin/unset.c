
#include "minishell.h"

int	ft_unset(char **cmd)
{
	int	i;

	if (cmd[1] == NULL)
		return (0);
	i = 1;
	while (cmd[i])
	{
		if (is_key_in_env_list(cmd[i]))
			delete_node(cmd[i]);
		i++;
	}
	return (0);
}

void	delete_node(char *key)
{
	t_env	*temp;
	t_env	*del_node;

	temp = g_info.env_list;
	del_node = get_node(key);
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

t_env	*get_node(char *key)
{
	t_env	*temp;

	temp = g_info.env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
