/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajeong <hajeong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 18:35:37 by jungchoi          #+#    #+#             */
/*   Updated: 2022/12/25 01:47:44 by hajeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		size;
	char	*s2;

	size = ft_strlen(s1);
	s2 = (char *)malloc(sizeof(char) * (size + 1));
	if (!s2)
		return (0);
	if (size == 0)
	{
		s2[0] = '\0';
		return (s2);
	}
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
