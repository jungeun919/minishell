/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 09:58:22 by hajeong           #+#    #+#             */
/*   Updated: 2023/01/09 20:05:03 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_env(char *before, char *value, char *after)
{
	char	*str;
	char	*join;

	if (!value)
		value = ft_strdup("");
	if (!before | !value | !after)
		return (0);
	str = (char *)malloc(sizeof(char) * (ft_strlen(before) \
	+ ft_strlen(value) + ft_strlen(after) + 1));
	if (!str)
		return (NULL);
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
void	replace_env(t_list *l_tok, t_env *envlst)
{
	char	*key;
	char	*key_end;
	char	*str;

	while (l_tok != NULL)
	{
		while (l_tok != NULL && l_tok->label >= AFTER_HEREDOC)
			l_tok = l_tok->next;
		if (l_tok != 0 && l_tok->label != 2 && ft_strchr(l_tok->content, '$'))
		{
			key = ft_strchr(l_tok->content, '$');
			key_end = key + 1;
			while (ft_isalnum(*key_end))
				key_end++;
			key = ft_substr(key, 1, (int)(key_end - key - 1));
			str = join_env(l_tok->content, get_env_value(envlst, key), key_end);
			free(l_tok->content);
			l_tok->content = str;
			free(key);
			if (l_tok->content == NULL)
				clear_lexer_and_exit(&l_tok);
		}
		if (l_tok && (!ft_strchr(l_tok->content, '$') || l_tok->label == 2))
			l_tok = l_tok->next;
	}
}
