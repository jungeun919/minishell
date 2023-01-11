/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:19:48 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/11 20:43:48 by sanghan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_heredoc_input(t_exec_token *token, t_env *env_list, int len)
{
	int		i;
	t_list	*in;

	i = 0;
	while (i < len)
	{
		in = token[i].parser_token->in;
		while (in != NULL)
		{
			if (ft_strncmp(in->content, "<<", 3) == 0)
			{
				get_infile(g_info.heredoc_cnt, in->next->content, env_list);
				g_info.heredoc_cnt++;
			}
			in = in->next->next;
		}
		i++;
	}
}

void	get_infile(int num, char *limiter, t_env *env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("fork error\n", 1);
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sig_handler);
		heredoc_child_process(num, limiter, env_list);
		exit(0);
	}
	wait(&status);
}

void	heredoc_child_process(int num, char *limiter, t_env *env_list)
{
	int		fd;
	char	*filename;
	char	*line;

	signal(SIGINT, SIG_IGN);
	filename = ft_strjoin("/tmp/", ft_itoa(num));
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_exit("open error\n", 1);
	line = readline("> ");
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		line = replace_env_heredoc(line, env_list);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	close(fd);
}
