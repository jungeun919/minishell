/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:38:06 by sanghan           #+#    #+#             */
/*   Updated: 2022/07/18 17:23:27 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

typedef enum e_result
{
	ERROR = 0,
	CONTINUE,
	SUCCESS
}t_result;

typedef struct s_gnl_lst
{
	ssize_t				idx;
	ssize_t				buf_size;
	char				*buf;
	struct s_gnl_lst	*prev;
	struct s_gnl_lst	*next;
}t_gnl_lst;

char		*get_next_line(int fd);
t_result	get_list(t_gnl_lst **head, t_gnl_lst **fd_lst, int fd);
void		set_fd_lst(t_gnl_lst **fd_lst, t_gnl_lst *prev, int fd);
t_result	read_str(t_gnl_lst **fd_lst, int fd);
t_result	join_str(t_gnl_lst **fd_lst, char **str, ssize_t read_size);

void		*ft_memcpy(void *dst, const void *src, size_t n);
t_result	free_lst(t_gnl_lst **head, t_gnl_lst **fd_lst);
char		*ret_str(t_gnl_lst **head, t_gnl_lst **fd_lst);
ssize_t		get_ret_len(t_gnl_lst **fd_lst);
char		*set_save(t_gnl_lst **fd_lst, char **ret, ssize_t ret_len);

#endif
