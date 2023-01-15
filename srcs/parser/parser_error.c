/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungchoi <jungchoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 12:30:23 by jungchoi          #+#    #+#             */
/*   Updated: 2023/01/15 14:41:05 by jungchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_parser_and_exit(t_parser_token *parser_token, int len)
{
	free_parser_token(parser_token, len);
	free_env_list(&(g_info.env_list));
	exit(0);
}
