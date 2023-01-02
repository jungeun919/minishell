/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 09:58:22 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/02 12:58:12 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// parser 2-1 -> 따옴표 지우기
void	remove_quote(t_list **lexer_token)
{
	t_list	*temp;

	temp = *lexer_token;
	while (temp != NULL)
	{
		if (temp->label % 9 == SINGLE_QUOTE || temp->label % 9 == DOUBLE_QUOTE)
		{
			ft_memmove(temp->content, temp->content + 1, \
			ft_strlen(temp->content) - 2);
			temp->content[ft_strlen(temp->content) - 2] = '\0';
			temp->label = 9 * (temp->label / 9);
		}
		temp = temp->next;
	}
}
