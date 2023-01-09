/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 12:57:51 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 13:12:04 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define ODD_QUOTE_ERROR 1
# define NO_STR_AFTER_REDIR_ERROR 2
# define DOUBLE_PIPE_ERROR 3


int	parsing(t_list **lexer_token, char *cmd, t_env *env_list)
{
	lexer(cmd, lexer_token);
	labeling(*lexer_token);
	labeling_after_heredoc(*lexer_token);
	if (check_odd_quote(*lexer_token))
		return (ODD_QUOTE_ERROR);
	replace_env(*lexer_token, env_list);
	remove_quote(lexer_token);
	merge_string(lexer_token);
	delete_blank(lexer_token);
	if (check_redirection(*lexer_token))
		return (NO_STR_AFTER_REDIR_ERROR);
	if (check_double_pipe(*lexer_token))
		return (DOUBLE_PIPE_ERROR);
	return (0);
}

int	parsing_error_handle(int code, t_list **lexer_token)
{
	if (code == 0)
		return (0);
	if (code == ODD_QUOTE_ERROR)
		printf("odd quote is not allowed!\n");
	else if (code == NO_STR_AFTER_REDIR_ERROR)
		printf("no string after redirection!\n");
	else if (code == DOUBLE_PIPE_ERROR)
		printf("double pipe!\n");
	ft_lstclear(lexer_token, free);
	return (code);
}