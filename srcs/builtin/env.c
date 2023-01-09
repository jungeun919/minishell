
#include "minishell.h"

// return value -> 0 : success, 1 : fail
int	ft_env(char **cmd, t_env *env_list)
{
	if (cmd[1] != NULL)
		return (error_return("too many arguments\n"));
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
