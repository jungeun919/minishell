/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:27:57 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/11 20:49:12 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*read_cmd(void)
{
	char	*cmd;

	set_echoctl_off();
	setting_signal();
	cmd = readline("minishell$ ");
	if (!cmd)
	{
		printf("\033[1A");
		printf("\033[11C");
		printf("exit");
		free_env_list(&(g_info.env_list));
		exit(0);
	}
	return (cmd);
}

void	show_shanghai(void)
{
	int		fd;
	char	*line;

	fd = open("/Users/han/temp/0111/srcs/utils/shanghai.txt", O_RDONLY);
	if (!fd)
		return ;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		ft_putstr_fd(COLOR_YELLOW, STDIN);
		printf("%s", line);
		free(line);
	}
	ft_putendl_fd(line, STDOUT);
	close(fd);
	free(line);
	ft_putendl_fd(END_COLOR, STDOUT);
	return ;
}
