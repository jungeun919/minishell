/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungeun <jungeun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 09:58:22 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 17:12:48 by jungeun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 파이프 갯수로 파서 토큰 갯수 얻기
int	parser_token_size(t_list *lexer_token)
{
	int	len;

	len = 1;
	while (lexer_token != NULL)
	{
		if (lexer_token->label == PIPE)
			len++;
		lexer_token = lexer_token->next;
	}
	return (len);
}

// 파서토큰 메모리 할당 받기
t_parser_token	*init_parser_token(int size)
{
	t_parser_token	*parser_token;
	int				i;

	parser_token = (t_parser_token *)malloc(sizeof(t_parser_token) * size);
	if (parser_token == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		parser_token[i].cmd = NULL;
		parser_token[i].in = NULL;
		parser_token[i].out = NULL;
		i++;
	}
	return (parser_token);
}

// 파서토큰 연결해주기 (우선 각 파서토큰의 cmd에 모든 연결리스트 노드 연결)
void	make_parser_token(t_list **lexer_token, t_parser_token *parser_token)
{
	t_list	*prev;
	t_list	*temp;
	t_list	*del;
	int		i;

	temp = *lexer_token;
	prev = temp;
	parser_token[0].cmd = temp;
	i = 0;
	while (temp != NULL)
	{
		if (temp->label == PIPE)
		{
			del = temp;
			parser_token[++i].cmd = temp->next;
			prev->next = NULL;
			ft_lstdelone(del, free);
		}
		prev = temp;
		temp = temp->next;
	}
}

// 파서 토큰 할당 해제 해주는 함수
void	free_parser_token(t_parser_token *parser_token, int len)
{
	int		i;

	i = 0;
	while (i < len)
	{
		if (parser_token[i].cmd != NULL)
			ft_lstclear(&(parser_token[i].cmd), free);
		if (parser_token[i].in != NULL)
			ft_lstclear(&(parser_token[i].in), free);
		if (parser_token[i].out != NULL)
			ft_lstclear(&(parser_token[i].out), free);
		i++;
	}
	free(parser_token);
}
