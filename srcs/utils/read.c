
#include "minishell.h"

char	*read_cmd()
{
	char	*cmd;

	set_echoctl_off();
	setting_signal();
	cmd = readline("minishell$ ");
	if (!cmd) // ctrl + d
	{
		printf("\033[1A"); // 커서를 위로 한 줄 올리기
		printf("\033[11C"); // 커서를 앞으로 11만큼 전진시키기
		printf("exit");
		exit(0);
	}
	return (cmd);
}
