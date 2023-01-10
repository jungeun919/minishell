/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:11:20 by sanghan           #+#    #+#             */
/*   Updated: 2022/07/18 17:23:26 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_gnl_lst	*head;
	t_gnl_lst			*fd_lst;

	fd_lst = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	if (!get_list(&head, &fd_lst, fd))
		return (0);
	if (!read_str(&fd_lst, fd))
	{
		if (free_lst(&head, &fd_lst))
			return (NULL);
	}
	return (ret_str(&head, &fd_lst));
}

t_result	get_list(t_gnl_lst **head, t_gnl_lst **fd_lst, int fd)
{
	t_gnl_lst	*prev;

	prev = NULL;
	if (*head)
	{
		*fd_lst = *head;
		while (*fd_lst && (*fd_lst)->idx != fd)
		{
			prev = *fd_lst;
			*fd_lst = (*fd_lst)->next;
		}
		if (*fd_lst && (*fd_lst)->idx == fd)
			return (CONTINUE);
	}
	*fd_lst = (t_gnl_lst *)malloc(sizeof(t_gnl_lst));
	if (!(*fd_lst))
		return (ERROR);
	set_fd_lst(fd_lst, prev, fd);
	if (prev)
		prev->next = *fd_lst;
	else
		*head = *fd_lst;
	return (SUCCESS);
}

void	set_fd_lst(t_gnl_lst **fd_lst, t_gnl_lst *prev, int fd)
{
	(*fd_lst)->idx = fd;
	(*fd_lst)->buf_size = 0;
	(*fd_lst)->buf = 0;
	(*fd_lst)->prev = prev;
	(*fd_lst)->next = NULL;
}

t_result	read_str(t_gnl_lst **fd_lst, int fd)
{
	char		*str;
	ssize_t		read_size;
	t_result	state;

	str = (char *)malloc(BUFFER_SIZE);
	if (!str)
		return (ERROR);
	read_size = read(fd, str, BUFFER_SIZE);
	if (read_size < 0 || (read_size == 0 && (*fd_lst)->buf_size == 0))
	{
		free(str);
		return (ERROR);
	}
	if (read_size == 0)
	{
		free(str);
		return (CONTINUE);
	}
	state = join_str(fd_lst, &str, read_size);
	if (state == SUCCESS)
		return (state);
	if (state == CONTINUE)
		return (read_str(fd_lst, fd));
	return (ERROR);
}

t_result	join_str(t_gnl_lst **fd_lst, char **str, ssize_t read_size)
{
	char	*temp;
	ssize_t	i;

	i = 0;
	temp = (char *)malloc((*fd_lst)->buf_size + read_size);
	if (!temp)
		return (ERROR);
	ft_memcpy(temp, (*fd_lst)->buf, (*fd_lst)->buf_size);
	ft_memcpy(temp + (*fd_lst)->buf_size, *str, read_size);
	free(*str);
	free((*fd_lst)->buf);
	(*fd_lst)->buf_size = (*fd_lst)->buf_size + read_size;
	(*fd_lst)->buf = temp;
	while (i < (*fd_lst)->buf_size)
	{
		if (temp[i] == '\n')
			return (SUCCESS);
		i++;
	}
	return (CONTINUE);
}
