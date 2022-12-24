
#include "minishell.h"

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*temp;

	if (!key) // 정해성이 추가했음, replace_env함수에서 substr 실패시 이 로직이 있으면 유용해 보임
		return (NULL);
	temp = env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
