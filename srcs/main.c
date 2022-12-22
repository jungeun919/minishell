
#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	printf("%c\n", *argv[0]);
	printf("%c\n", *envp[0]);
	
	/* get command line */
	char *cmd;
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
		if (ft_strlen(cmd) >= 1)
			add_history(cmd);
		free(cmd);
		cmd = NULL;
	}
	/* get command line */

	return (0);
}
