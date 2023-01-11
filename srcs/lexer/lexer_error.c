/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 12:30:23 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/11 16:42:40 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_temp_clear_and_exit(t_list **lexer_token, char *temp)
{
	if (temp != NULL)
		free(temp);
	ft_lstclear(lexer_token, free);
	free_env_list(&(g_info.env_list));
	exit(0);
}

void	clear_lexer_and_exit(t_list **lexer_token)
{
	ft_lstclear(lexer_token, free);
	free_env_list(&(g_info.env_list));
	exit(0);
}
