/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:38:14 by sanghan           #+#    #+#             */
/*   Updated: 2022/07/18 17:23:26 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	tmp_dst = (unsigned char *) dst;
	tmp_src = (unsigned char *) src;
	if (dst == 0 && src == 0)
		return (dst);
	i = 0;
	while (i < n)
	{
		tmp_dst[i] = tmp_src[i];
		i++;
	}
	return (dst);
}

t_result	free_lst(t_gnl_lst **head, t_gnl_lst **fd_lst)
{
	t_gnl_lst	*fd_prev;
	t_gnl_lst	*fd_next;

	fd_prev = (*fd_lst)->prev;
	fd_next = (*fd_lst)->next;
	if ((*fd_lst)->buf_size != 0)
		free((*fd_lst)->buf);
	if ((*fd_lst)->idx == (*head)->idx)
		*head = fd_next;
	if (fd_prev)
		fd_prev->next = fd_next;
	if (fd_next)
		fd_next->prev = fd_prev;
	free(*fd_lst);
	if (!fd_next)
		*fd_lst = 0;
	return (SUCCESS);
}

char	*ret_str(t_gnl_lst **head, t_gnl_lst **fd_lst)
{
	char	*ret;
	char	*save;
	ssize_t	ret_len;

	ret_len = get_ret_len(fd_lst);
	ret = (char *)malloc(ret_len + 1);
	if (!ret)
	{
		if (free_lst(head, fd_lst))
			return (NULL);
	}
	ft_memcpy(ret, (*fd_lst)->buf, ret_len);
	ret[ret_len] = '\0';
	if ((*fd_lst)->buf_size - ret_len)
	{
		save = set_save(fd_lst, &ret, ret_len);
		if (!save)
			return (NULL);
	}
	else
		free_lst(head, fd_lst);
	return (ret);
}

ssize_t	get_ret_len(t_gnl_lst **fd_lst)
{
	ssize_t	ret_len;

	ret_len = 0;
	while (ret_len < (*fd_lst)->buf_size)
	{
		if ((*fd_lst)->buf[ret_len++] == '\n')
			break ;
	}
	return (ret_len);
}

char	*set_save(t_gnl_lst **fd_lst, char **ret, ssize_t ret_len)
{
	char	*save;

	save = (char *)malloc((*fd_lst)->buf_size - ret_len);
	if (!save)
	{
		free(*ret);
		return (NULL);
	}
	ft_memcpy(save, (*fd_lst)->buf + ret_len, (*fd_lst)->buf_size - ret_len);
	free((*fd_lst)->buf);
	(*fd_lst)->buf_size = (*fd_lst)->buf_size - ret_len;
	(*fd_lst)->buf = save;
	return (save);
}
