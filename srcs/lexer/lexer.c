
#include "minishell.h"

# define NOMAL_STRING 0
# define DOUBLE_QUOTE 1
# define SINGLE_QUOTE 2
# define BLANK 3
# define PIPE 4
# define REDIR_IN 5
# define REDIR_HEREDOC 6
# define REDIR_OUT 7
# define REDIR_DOUBLE_OUT 8


t_env	*init_env_list(char **envp);
t_env	*make_env_node(char *key, char *value);
void	env_list_add_node(t_env **list, t_env *node);
char	*get_env_value(t_env *env_list, char *key);

void free_temp_clear_and_exit(t_list **lexer_token, char *temp)
{
	free(temp);
	ft_lstclear(lexer_token, free);
	// env_list 도 free 해줄것
	exit(0);
}

void clear_and_exit(t_list **lexer_token)
{
	ft_lstclear(lexer_token, free);
	// env_list 도 free 해줄것
	exit(0);
}

void lexer(char *str, t_list **lexer_token)
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
		;
		// return lexer(str+1, lexer_token);
	else if (str[0] != '<' && str[0] != '>' && str[0] != '|')
		while (!ft_strchr("|\'\"><\t\n\v\f\r ", str[i + 1]))
			i++;
	temp = ft_substr(str, 0, i+1);
	new = ft_lstnew(temp);
	if (temp == NULL || new == NULL)
		free_temp_clear_and_exit(lexer_token, temp); // ft_lstclear(lexer_token, free); // temp free하기 + exit() 하는 함수 필요
	ft_lstadd_back(lexer_token, new);
	if (str[i] == '\0')
		return ;
	lexer(str+i+1, lexer_token);
}

void labeling(t_list *lexer_token)
{
	t_list	*temp;
	char	ch;

	temp = lexer_token;
	while (temp != NULL)
	{
		ch = temp->content[0];
		if (ch == '\"')
			temp->label = DOUBLE_QUOTE;
		else if (ch == '\'')
			temp->label = SINGLE_QUOTE;
		else if (ft_strchr("\t\n\v\f\r ",ch))
			temp->label = BLANK;
		else if (ch == '|')
			temp->label = PIPE;
		else if (ch == '<')
		{
			temp->label = REDIR_IN;
			if (temp->content[1] == '<')
				temp->label = REDIR_HEREDOC;
		}
		else if (ch == '>')
		{
			temp->label = REDIR_OUT;
			if (temp->content[1] == '>')
				temp->label = REDIR_DOUBLE_OUT;
		}
		temp = temp->next;
	}
}

static char	*ft_join_env_and_free(char *before,  char *value, char *after)
{
	char *str;
	char *join;

	if (!value)
		value = ft_strdup("");
	if (!before | !value | !after)
		return (0);
	str = (char *)malloc(sizeof(char) * (ft_strlen(before) + ft_strlen(value) + ft_strlen(after) + 1));
	if (!str)
		return 0;
	join = str;
	while (*before && *before != '$')
		*join++ = *before++;
	while (*value)
		*join++ = *value++;
	while (*after)
		*join++ = *after++;
	*join = '\0';
	return (str);
}

// parser 1 -> 환경변수 교체
void replace_env(t_list *lexer_token, t_env *env_list)
{
	t_list *temp;
	char	*key;
	char	*key_end;
	char	*str;
	
	temp = lexer_token;
	while (temp != NULL)
	{
		if (temp->label != SINGLE_QUOTE && ft_strchr(temp->content, '$'))
		{
			key = ft_strchr(temp->content, '$'); // $의 인덱스
			// *key = '\0';
			// key_idx = (int)(key - temp->content);
			key_end = key + 1; // 환경변수 끝 하나 뒤 인덱스 ("012$PATH 345") <- key = '$', key_end = ' '
			while (ft_isalnum(*key_end))
				key_end++;
			key = ft_substr(key, 1, (int)(key_end - key - 1)); // $다음 인덱스 부터 저장
			str = ft_join_env_and_free(temp->content, get_env_value(env_list,key), key_end);
			temp->content = str;
			free(key);
			// join_and_free 실패시 -> temp->content = NULL [ft_lstclear로 초기화 하고 exit]
			if (temp->content == NULL)
				clear_and_exit(&lexer_token);
		}
		if (!ft_strchr(temp->content, '$') || temp->label == SINGLE_QUOTE)
			temp = temp->next;
	}
}

// parser 2-1 -> 따옴표 지우기
void remove_quote(t_list **lexer_token)
{
	t_list	*temp;

	temp = *lexer_token;
	while (temp != NULL)
	{
		if (temp->label == SINGLE_QUOTE || temp->label == DOUBLE_QUOTE)
		{
			ft_memmove(temp->content, temp->content + 1, ft_strlen(temp->content) - 2);
			temp->content[ft_strlen(temp->content) - 2] = '\0';
			temp->label = NOMAL_STRING;
		}
		temp = temp->next;
	}
}

// parser 2-2 -> 문자열 병합
void merge_string(t_list **lexer_token)
{
	t_list	*temp;
	t_list	*colony;
	char	*str;

	temp = *lexer_token;
	while (temp != NULL)
	{
		if (temp->label == NOMAL_STRING)
		{
			colony = temp->next;
			while (colony->label == NOMAL_STRING)
			{
				str = ft_strjoin(temp->content, colony->content);
				free(temp->content);
				temp->content = str;
				temp->next = colony->next;
				ft_lstdelone(colony, free);
				colony = temp->next;
			}
		}
		temp = temp->next;
	}
}

// parser 2-3 -> blank 중복 제거
void delete_duplicate_blank(t_list **lexer_token)
{
	t_list	*prev;
	t_list	*temp;
	t_list	*del;

	temp = *lexer_token;
	prev = temp;
	while (temp->next != NULL)
	{
		if (temp->label == BLANK)
		{
			if (temp->next->label == BLANK)
			{
				del = temp->next;
				temp->next = del->next;
				ft_lstdelone(del, free);
				temp = prev;
				continue ;
			}
		}
		prev = temp;
		temp = temp->next;
	}
}


//parser 2 -> 따옴표 제거 하면서 공백이 없었다면 붙여주기 -> 너무 어렵다...
// void remove_quote(t_list **lexer_token, char *str, char *quote_pointer)
// {
// 	t_list	*before;
// 	t_list	*now;
// 	char 	quote;

// 	now = *lexer_token;
// 	before = now;
// 	while (now != NULL) // before 뒤의 문자열의 따옴표 검사
// 	{
// 		quote = now->content[0];
// 		if (quote == '\'' || quote == '\"')
// 		{
// 			if (quote != now->content[ft_strlen(before->content) - 1])
// 				; // single quote error!
// 			quote_pointer = ft_strchr(quote_pointer + 1, quote);
// 			ft_memmove(now->content, now->content + 1, ft_strlen(now->content) - 2);
// 			// before->content = ft_join_and_free(before->content, ft_substr(now->content, 1, ft_strlen(now->content) - 1));
// 			now->content[ft_strlen(now->content) - 2] = '\0';
// 			// printf(" str[(int)(quote - str) - 1] == [%c],  (int)(quote - str) - 1 = [%d]\n",str[(int)(quote - str) - 1] ,(int)(quote - str) - 1);
// 			if (!ft_strchr(" \t\n\v\f\r|<>", str[(int)(quote_pointer - str) - 1]))
// 			{
// 				before->next = now->next;
// 				before->content = ft_strjoin(before->content, now->content);
// 				ft_lstdelone(now, free);
// 				now = now->next;
// 				// quote_pointer = ft_strchr(quote_pointer + 1, quote);
// 				// // continue ;
// 			}
// 			printf("%c %d\n", str[(int)(quote_pointer - str) - 1], (int)(quote_pointer - str));
// 			if (!ft_strchr(" \t\n\v\f\r|<>", str[(int)(quote_pointer - str) - 1]))
// 			{
// 				before->next = now->next;
// 				before->content = ft_strjoin(before->content, now->content);
// 				ft_lstdelone(now, free);
// 				now = now->next;
// 			}
// 			quote_pointer = ft_strchr(quote_pointer + 1, quote);
// 			continue;
// 		}
// 		before = now;
// 		now = now->next;
// 	}
// }

int	main(int argc, char *argv[], char *envp[]) {
	argc++; argc--;
	argv += 0;
	t_list *lexer_token = NULL;
	// lexer("  cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2 ", &lexer_token);
	
	char str[100] = "echo    '321'$USER     \"123\"\"$PATH\"   \'$USER\' $USER$USER\"$USER$USER\" ";
	
	lexer(str, &lexer_token);
	labeling(lexer_token);
	t_env	*env_list;
	env_list = init_env_list(envp);
	
	replace_env(lexer_token, env_list);

	remove_quote(&lexer_token);

	merge_string(&lexer_token);
	
	delete_duplicate_blank(&lexer_token);

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

