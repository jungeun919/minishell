/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghan <sanghan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:27:57 by sanghan           #+#    #+#             */
/*   Updated: 2023/01/12 05:11:28 by sanghan          ###   ########.fr       */
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
	ft_putstr_fd(COLOR_YELLOW, STDIN);
	ft_putstr_fd("..........,.............................\n", STDERR);
	ft_putstr_fd("........................................\n", STDERR);
	ft_putstr_fd("..........~.............................\n", STDERR);
	ft_putstr_fd("..........!.............................\n", STDERR);
	ft_putstr_fd("..........!................!............\n", STDERR);
	ft_putstr_fd("..........!.....MINISHELL..!............\n", STDERR);
	ft_putstr_fd("..........*:...............;............\n", STDERR);
	ft_putstr_fd("..........*............,,..;............\n", STDERR);
	ft_putstr_fd("..........:............::..~............\n", STDERR);
	ft_putstr_fd("..........!.........,..::..;............\n", STDERR);
	ft_putstr_fd("..........:.........;,:,;..!............\n", STDERR);
	ft_putstr_fd("..........:.-,.:,...:-;,:..!-,..,.......\n", STDERR);
	ft_putstr_fd(".....-..,.:!::.-,...;;;-;,!:;;~.;~,.....\n", STDERR);
	ft_putstr_fd("....~~~!~*~~=:,-,..~::~~;,*;*;*,!,-.....\n", STDERR);
	ft_putstr_fd("..,;~-~!~*!:*~**:,.*~!;~!-:;***,*,!,....\n", STDERR);
	ft_putstr_fd("..-~~-:!;!!;*;!!~!,*~!;~!~==!==-!~:~.,..\n", STDERR);
	ft_putstr_fd(",.**~;!=!*;:!!*=~*:*:*;~=$$$=!=~!~:~~~;~\n", STDERR);
	ft_putstr_fd("!:!*;;*$**#;$*$#=*=#$$$$#$$*:$$!$;!**!*!\n", STDERR);
	ft_putstr_fd("~~---~-~~:;::~~~~-~::::~~~:;::~-~---,,,,\n", STDERR);
	ft_putendl_fd(END_COLOR, STDOUT);
}
