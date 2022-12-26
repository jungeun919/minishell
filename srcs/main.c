
#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 1)
	{
		ft_putstr_fd("argument error\n", STDERR_FILENO);
		return (0);
	}
	printf("%c\n", *argv[0]);

	t_env	*env_list;
	env_list = init_env_list(envp);

	/* get_env_value test */
	char	*return_value;
	return_value = get_env_value(env_list, "PATH");
	printf("%s\n", return_value);
	/* get_env_value test */

	/* set ECHOCTL on/off */
	struct termios	term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL); // 제어문자 반향
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // 변경 속성 바로 적용
	/* set ECHOCTL on/off */

	setting_signal();
	
	/* get command line */
	char *cmd;
	while (1)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
		{
			printf("\033[1A"); // 커서를 위로 한 줄 올리기
			printf("\033[11C"); // 커서를 앞으로 10만큼 전진시키기
			printf("exit");
			exit(0);
		}

		if (ft_strncmp(cmd, "env", 4) == 0)
			ft_env(env_list);

		if (ft_strlen(cmd) >= 1)
			add_history(cmd);
		free(cmd);
		cmd = NULL;
	}
	/* get command line */

	return (0);
}
