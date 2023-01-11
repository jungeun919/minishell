/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:28:06 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/11 20:47:39 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_2d_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	error_exit(char *str, int status)
{
	ft_putstr_fd(str, STDERR_FILENO);
	exit(status);
}

int	error_return(char *str)
{
	ft_putstr_fd(str, STDERR_FILENO);
	return (1);
}

int	free_init_exec_info(pid_t **pids, int ***fds, int i)
{
	while (i >= 0)
	{
		free((*fds)[i]);
		i--;
	}
	if (*pids != NULL)
		free(*pids);
	if (*fds != NULL)
		free(*fds);
	return (1);
}

void	rm_all_heredoc_file(void)
{
	int		i;
	char	*filename;

	i = 0;
	while (i < g_info.heredoc_cnt)
	{
		filename = ft_strjoin("/tmp/", ft_itoa(i));
		fprintf(stderr, "unlink %s\n", filename);
		unlink(filename);
		i++;
	}
}
