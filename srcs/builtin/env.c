
#include "minishell.h"

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
