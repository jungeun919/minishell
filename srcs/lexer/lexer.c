
#include "minishell.h"

t_env	*init_env_list(char **envp);
t_env	*make_env_node(char *key, char *value);
void	env_list_add_node(t_env **list, t_env *node);
char	*get_env_value(t_env *env_list, char *key);

void free_temp_clear_and_exit(t_list **lexer_tokens, char *temp)
{
	free(temp);
	ft_lstclear(lexer_tokens, free);
	// env_list 도 free 해줄것
	exit(0);
}

void clear_and_exit(t_list **lexer_tokens)
{
	ft_lstclear(lexer_tokens, free);
	// env_list 도 free 해줄것
	exit(0);
}

void lexer(char *str, t_list **lexer_tokens)
{
	int		i;
	char	*temp;
	t_list	*new;

	i = 0;
	if (str[0] == '\0')
		return ;
	else if (str[0] == '\"' || str[0] == '\'')
		while (str[++i] != '\0' && str[i] != str[0])
			;
	else if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		i++;
	else if (ft_strchr("\t\n\v\f\r ", str[0]))
		return lexer(str+1, lexer_tokens);
	else if (str[0] != '<' && str[0] != '>' && str[0] != '|')
		while (!ft_strchr("|\'\"><\t\n\v\f\r ", str[i + 1]))
			i++;
	temp = ft_substr(str, 0, i+1);
	new = ft_lstnew(temp);
	if (temp == NULL || new == NULL)
		free_temp_clear_and_exit(lexer_tokens, temp); // ft_lstclear(lexer_tokens, free); // temp free하기 + exit() 하는 함수 필요
	ft_lstadd_back(lexer_tokens, new);
	if (str[i] == '\0')
		return ;
	lexer(str+i+1, lexer_tokens);
}

static char	*ft_join_and_free(char *buffer, char *buf)
{
	char	*temp;

	temp = ft_strjoin(buffer, buf);
	free(buffer);
	return (temp);
}

void replace_env(t_list *lexer_token, t_env *env_list)
{
	t_list *temp;
	char	*key;
	char	*key_end;
	
	temp = lexer_token;
	while (temp != NULL)
	{
		if (temp->content[0] !='\'' && ft_strchr(temp->content, '$'))
		{
			key = ft_strchr(temp->content, '$'); // $의 인덱스
			key_end = key + 1; // 환경변수 끝 하나 뒤 인덱스 ("012$PATH 345") <- key = '$', key_end = ' '
			while (ft_isalnum(*key_end))
				key_end++;
			temp->content = ft_substr(temp->content, 0, (int)(key - temp->content));
			// substr 할당 실패시 temp->content = NULL -> ft_join_and_free에서 0 리턴 -> join_and_free에서 0리턴 -> temp->content = NULL [ft_lstclear로 초기화 하고 exit]
			key = ft_substr(key, 1, (int)(key_end - key - 1)); // $다음 인덱스 부터 저장
			// ft_substr에서 할당 실패시 -> key = NULL -> get_env_value NULL 리턴 -> ft_join_and_free에서 0 리턴 -> join_and_free에서 0리턴 -> temp->content = NULL [ft_lstclear로 초기화 하고 exit]
			temp->content = ft_join_and_free(temp->content, ft_strdup(get_env_value(env_list, key)));
			// ft_join_and_free에서 실패시  temp->content = NULL -> join_and_free에서 0리턴 -> temp->content = NULL [ft_lstclear로 초기화 하고 exit]
			free(key);
			temp->content = ft_join_and_free(temp->content, ft_strdup(key_end));
			// join_and_free 실패시 -> temp->content = NULL [ft_lstclear로 초기화 하고 exit]
			if (temp->content == NULL)
				clear_and_exit(&lexer_token);
		}
		if (!ft_strchr(temp->content, '$') || temp->content[0] =='\'')
			temp = temp->next;
	}
}

int	main(int argc, char *argv[], char *envp[]) {
	argc++; argc--;
	argv += 0;
	t_list *lexer_token = NULL;
	// lexer("  cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2 ", &lexer_token);
	
	char str[100] = "echo $USER $PATH asdf123";
	
	lexer(str, &lexer_token);
	
	t_env	*env_list;
	env_list = init_env_list(envp);
	
	replace_env(lexer_token, env_list);
	
	t_list *temp = lexer_token;
	int node_index = 0;
	while (temp != NULL)
	{
		printf("%2d 번째 노드 : [%s]\n", node_index++, temp->content);
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

