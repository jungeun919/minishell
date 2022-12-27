
#include "minishell.h"

int	ft_env(t_execute_unit *argv, t_env *env_list)
{
	if (argv->next != NULL)
		return (0); // too many argument
	print_env_list(env_list);
	return (0);
}

void	print_env_list(t_env *env_list)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}
