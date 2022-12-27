
#include "minishell.h"

// 리턴값 정의 필요
int	ft_export(t_execute_unit *argv, t_env *env_list)
{
	t_execute_unit	*temp;
	int				i;
	char			*key;
	char			*value;

	temp = argv->next;
	if (temp == NULL) // export만 입력했을 때 목록 표시
		print_env_list(env_list);
	while (temp)
	{
		i = 0;
		while (temp->command[i] && temp->command[i] != '=')
			i++;
		key = ft_substr(temp->command, 0, i); // 할당 실패했을 때 처리할건지? 일단 함수 안에서 널 체크하는 정도로만 작성
		value = ft_substr(temp->command, i + 1, ft_strlen(temp->command) - i - 1);
		execute_export(key, value, env_list);
		temp = temp->next;
	}
	return (0);
}

void	execute_export(char *key, char *value, t_env *env_list)
{
	t_env	*node;

	if (is_key_in_env_list(key, env_list))
		update_value(key, value, &env_list);
	else
	{
		if (!(is_valid_format_key(key)))
		{
			free(key);
			free(value);
			return ;
		}
		node = make_env_node(key, value);
		if (!node)
			return ; // errno 설정 관련 추가?
		env_list_add_node(&env_list, node);
	}
}

int	is_key_in_env_list(char *key, t_env *env_list)
{
	t_env	*temp;

	if (!key)
		return (0);
	temp = env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	is_valid_format_key(char *key)
{
	int	i;

	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	update_value(char *key, char *value, t_env **env_list)
{
	t_env	*temp;

	if (!key || !value)
		return ;
	temp = *env_list;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(temp->value);
			temp->value = value;
		}
		temp = temp->next;
	}
}
