/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 09:58:22 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 20:06:05 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// parser 2-2 -> 문자열 병합
void	merge_string(t_list **lexer_token)
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
