/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 12:30:23 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/02 13:26:36 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_temp_clear_and_exit(t_list **lexer_token, char *temp)
{
	free(temp);
	ft_lstclear(lexer_token, free);
	// env_list 도 free 해줄것
	exit(0);
}

void	clear_and_exit(t_list **lexer_token)
{
	ft_lstclear(lexer_token, free);
	// env_list 도 free 해줄것
	exit(0);
}
