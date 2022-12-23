/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 14:43:51 by sanghan           #+#    #+#             */
/*   Updated: 2022/12/23 17:02:15 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	show_shanghai(void)
{
	int		fd;
	char	*line;

//	fd = open("/Users/han/42Seoul/cursus/minishell/make_minishell/includes/shanghai.txt", O_RDONLY);
	fd = open("/Users/sanghan/han/cursus/minishell/make_minishell/includes/shanghai.txt", O_RDONLY);
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
//pwd----------------------------------------------------------
void	ft_pwd(char *cmd)
{
	char	*path;

	if (cmd == NULL)
		return ;
	path = getcwd(NULL, 0);
	if (!path)
	{
		ft_putstr_fd("Error Path\n", STDERR_FILENO);
		return ;
	}
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(path);
	return ;
}
//--------------------------------------------------------------



int	main(int argc, char *argv [], char *envp[])
{
	char	**g_envp;
	char	*cmd;

	if (argc != 1)
	{
		printf("argument error\n");
		return (0);
	}
	g_envp = envp;
	(void)argc;
	(void)argv;
	(void)g_envp;
	show_shanghai();
	/* get command line */
	while (1)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
		{
			printf("\033[1A"); // 커서를 위로 한 줄 올리기
			printf("\033[10C"); // 커서를 앞으로 10만큼 전진시키기
			printf(" exit");
			exit(0);
		}
		ft_pwd(cmd);
		if (ft_strlen(cmd) >= 1)
			add_history(cmd);
		free(cmd);
		cmd = NULL;
	}
	/* get command line */

	return (0);
}
