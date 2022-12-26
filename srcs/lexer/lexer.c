/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungchoi <jungchoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 09:58:22 by hajeong           #+#    #+#             */
/*   Updated: 2022/12/26 15:32:06 by jungchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (lexer_token != NULL)
	{
		if (lexer_token->content[0] == '\"')
			lexer_token->label = DOUBLE_QUOTE;
		else if (lexer_token->content[0] == '\'')
			lexer_token->label = SINGLE_QUOTE;
		else if (ft_strchr("\t\n\v\f\r ",lexer_token->content[0]))
			lexer_token->label = BLANK;
		else if (lexer_token->content[0] == '|')
			lexer_token->label = PIPE;
		else if (lexer_token->content[0] == '<')
		{
			lexer_token->label = REDIR_IN;
			if (lexer_token->content[1] == '<')
				lexer_token->label = REDIR_HEREDOC;
		}
		else if (lexer_token->content[0] == '>')
		{
			lexer_token->label = REDIR_OUT;
			if (lexer_token->content[1] == '>')
				lexer_token->label = REDIR_DOUBLE_OUT;
		}
		lexer_token = lexer_token->next;
	}
}

int check_odd_quote(t_list *lexer_token) // 0 : 짝수 따옴표, 1 : 홀수 따옴표
{
	int len;

	while (lexer_token != NULL)
	{
		if (lexer_token->label == DOUBLE_QUOTE || lexer_token->label == SINGLE_QUOTE)
		{
			len = ft_strlen(lexer_token->content);
			if (len < 2)
				return (1);
			if (lexer_token->content[0] != lexer_token->content[len - 1])
				return (1);
		}			
		lexer_token = lexer_token->next;
	}
	return (0);
}

static char	*ft_join_env(char *before,  char *value, char *after)
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
	char	*key;
	char	*key_end;
	char	*str;
	
	while (lexer_token != NULL)
	{
		if (lexer_token->label != SINGLE_QUOTE && ft_strchr(lexer_token->content, '$'))
		{
			key = ft_strchr(lexer_token->content, '$'); // $의 인덱스
			key_end = key + 1; // 환경변수 끝 하나 뒤 인덱스 ("012$PATH 345") <- key = '$', key_end = ' '
			while (ft_isalnum(*key_end))
				key_end++;
			key = ft_substr(key, 1, (int)(key_end - key - 1)); // $다음 인덱스 부터 저장
			str = ft_join_env(lexer_token->content, get_env_value(env_list,key), key_end);
			lexer_token->content = str;
			free(key);
			if (lexer_token->content == NULL)
				clear_and_exit(&lexer_token);
		}
		if (!ft_strchr(lexer_token->content, '$') || lexer_token->label == SINGLE_QUOTE)
			lexer_token = lexer_token->next;
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
			while (colony != NULL && colony->label == NOMAL_STRING)
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

// parser 2-3 -> blank 제거
void delete_blank(t_list **lexer_token)
{
	t_list	*prev;
	t_list	*temp;

	while ((*lexer_token)->label == BLANK)
	{
		prev = *lexer_token;
		*lexer_token = (*lexer_token)->next;
		ft_lstdelone(prev, free);
	}
	temp = *lexer_token;
	prev = temp;
	while (temp != NULL)
	{
		if (temp->label == BLANK)
		{
			prev->next = temp->next;
			ft_lstdelone(temp, free);
			temp = prev->next;
			continue;
		}
		prev = temp;
		temp = temp->next;
	}
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	argc++; argc--;
// 	argv += 0;
// 	t_list *lexer_token = NULL;
// 	// lexer("  cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2 ", &lexer_token);
	
// 	// char str[100] = "\n      echo    '321'$USER     \"123\"\"$PATH\"   \'$USER\' $USER$USER\"$USER$USER\" ";
// 	t_env	*env_list;
// 	env_list = init_env_list(envp);
	
// 	char str[5000];

// 	t_list *temp = lexer_token;
// 	int node_index = 0;
// 	char node_label[20];

// 	while (1)
// 	{
// 		gets(str);

// 		//	lexer
// 		lexer(str, &lexer_token);
// 		labeling(lexer_token);
// 		if (check_odd_quote(lexer_token))
// 		{
// 			ft_lstclear(&lexer_token, free);
// 			printf("odd quote is not allowed!\n");
// 			continue ;
// 		}

// 		//	parsser
// 		replace_env(lexer_token, env_list);
// 		remove_quote(&lexer_token);
// 		merge_string(&lexer_token);
// 		delete_blank(&lexer_token);

// 		temp = lexer_token;
// 		node_index = 0;
// 		while (temp != NULL)
// 		{
						
// 			if      (temp->label == 0) ft_strlcpy( node_label , "NOMAL_STRING",20);
// 			else if (temp->label == 1) ft_strlcpy( node_label , "DOUBLE_QUOTE",20);
// 			else if (temp->label == 2) ft_strlcpy( node_label , "SINGLE_QUOTE",20);
// 			else if (temp->label == 3) ft_strlcpy( node_label , "BLANK",20);
// 			else if (temp->label == 4) ft_strlcpy( node_label , "PIPE",20);
// 			else if (temp->label == 5) ft_strlcpy( node_label , "REDIR_IN",20);
// 			else if (temp->label == 6) ft_strlcpy( node_label , "REDIR_HEREDOC",20);
// 			else if (temp->label == 7) ft_strlcpy( node_label , "REDIR_OUT",20);
// 			else if (temp->label == 8) ft_strlcpy( node_label , "REDIR_DOUBLE_OUT",20);
			
// 			printf("%2d 번째 노드 : [%s] <- (%s)\n", node_index++, temp->content, node_label);
// 			temp = temp->next;
// 		}

// 		ft_lstclear(&lexer_token, free);
// 	}
// }



// t_env	*init_env_list(char **envp)
// {
// 	int		i;
// 	char	**temp;
// 	t_env	*env_node;
// 	t_env	*env_list;

// 	i = 0;
// 	env_list = NULL; // 아무것도 안들어갔을 때 널 리턴
// 	while (envp[i] != NULL)
// 	{
// 		temp = ft_split(envp[i], '=');
// 		env_node = make_env_node(temp[0], temp[1]);
// 		if (!env_node)
// 		{
// 			ft_putstr_fd("malloc error\n", STDERR_FILENO);
// 			exit(1);
// 		}
// 		env_list_add_node(&env_list, env_node);
// 		free(temp);
// 		temp = NULL;
// 		i++;
// 	}
// 	return (env_list);
// }

// t_env	*make_env_node(char *key, char *value)
// {
// 	t_env	*env_node;

// 	env_node = (t_env *)malloc(sizeof(t_env));
// 	if (!env_node)
// 		return (NULL);
// 	env_node->key = key;
// 	env_node->value = value;
// 	env_node->next = NULL;
// 	return (env_node);
// }

// void	env_list_add_node(t_env **list, t_env *node)
// {
// 	t_env	*temp;

// 	if (!node || !list)
// 		return ;
// 	if (*list == NULL) // 첫번째 노드
// 		*list = node;
// 	else
// 	{
// 		temp = *list;
// 		while (temp->next != NULL)
// 			temp = temp->next;
// 		temp->next = node;
// 	}
// }

// char	*get_env_value(t_env *env_list, char *key)
// {
// 	t_env	*temp;

// 	if (!key) // 정해성이 추가했음, replace_env함수에서 substr 실패시 이 로직이 있으면 유용해 보임
// 		return (NULL);
// 	temp = env_list;
// 	while (temp)
// 	{
// 		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
// 			return (temp->value);
// 		temp = temp->next;
// 	}
// 	return (NULL);
// }
