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

# define NORMAL_STRING 0
# define DOUBLE_QUOTE 1
# define SINGLE_QUOTE 2
# define BLANK 3
# define PIPE 4
# define REDIR_IN 5
# define REDIR_HEREDOC 6
# define REDIR_OUT 7
# define REDIR_DOUBLE_OUT 8
# define AFTER_HEREDOC 9
# define AFTER_HEREDOC_DQ 10
# define AFTER_HEREDOC_SQ 11

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
		else if (lexer_token->content[0] == '<' || lexer_token->content[0] == '>')
		{
			lexer_token->label = REDIR_IN;
			if (lexer_token->content[0] == '>')
				lexer_token->label = REDIR_OUT;
			if (lexer_token->content[0] == lexer_token->content[1])
				(lexer_token->label)++;
		}
		lexer_token = lexer_token->next;
	}
}

void labeling_after_heredoc(t_list *lexer_token)
{
	int flag;

	flag = 0;
	while (lexer_token != NULL)
	{
		if (lexer_token->label == REDIR_HEREDOC)
			flag = 1;
		else if (flag == 1 && (lexer_token->label == NORMAL_STRING || lexer_token->label == DOUBLE_QUOTE || lexer_token->label == SINGLE_QUOTE))
		{
			while (lexer_token != NULL && (lexer_token->label == NORMAL_STRING || lexer_token->label == DOUBLE_QUOTE || lexer_token->label == SINGLE_QUOTE))
			{
				lexer_token->label += AFTER_HEREDOC;
				lexer_token = lexer_token->next;
			}
			flag = 0;
			continue ;
		}
		lexer_token = lexer_token->next;
	}
}

int check_odd_quote(t_list *lexer_token) // 0 : 짝수 따옴표, 1 : 홀수 따옴표
{
	int len;

	while (lexer_token != NULL)
	{ // 9 == AFTER_HEREDOC
		if (lexer_token->label % 9 == DOUBLE_QUOTE || lexer_token->label % 9 == SINGLE_QUOTE)
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
		while (lexer_token != NULL && lexer_token->label >= AFTER_HEREDOC)
			lexer_token = lexer_token->next;
		if (lexer_token != NULL && lexer_token->label != SINGLE_QUOTE && ft_strchr(lexer_token->content, '$'))
		{
			key = ft_strchr(lexer_token->content, '$'); // $의 인덱스
			key_end = key + 1; // 환경변수 끝 하나 뒤 인덱스 ("012$PATH 345") <- key = '$', key_end = ' '
			while (ft_isalnum(*key_end))
				key_end++;
			key = ft_substr(key, 1, (int)(key_end - key - 1)); // $다음 인덱스 부터 저장
			str = ft_join_env(lexer_token->content, get_env_value(env_list,key), key_end);
			free(lexer_token->content);
			lexer_token->content = str;
			free(key);
			if (lexer_token->content == NULL)
				clear_and_exit(&lexer_token);
		}
		if (lexer_token != NULL && (!ft_strchr(lexer_token->content, '$') || lexer_token->label == SINGLE_QUOTE))
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
		if (temp->label % 9 == SINGLE_QUOTE || temp->label % 9 == DOUBLE_QUOTE)
		{
			ft_memmove(temp->content, temp->content + 1, ft_strlen(temp->content) - 2);
			temp->content[ft_strlen(temp->content) - 2] = '\0';
			// temp->label = NORMAL_STRING;
			temp->label = 9 * (temp->label / 9);
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
		if (temp->label % 9 == NORMAL_STRING)
		{
			temp->label = NORMAL_STRING;
			colony = temp->next;
			while (colony != NULL && colony->label % 9 == NORMAL_STRING)
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

int check_redirection(t_list *lexer_token)
{
	while (lexer_token != NULL)
	{
		if (REDIR_IN <= lexer_token->label && lexer_token->label <= REDIR_DOUBLE_OUT)
		{
			if (lexer_token->next == NULL || lexer_token->next->label != NORMAL_STRING)
			{
				return (1);
			}
		}
		lexer_token = lexer_token->next;
	}
	return (0);
}
