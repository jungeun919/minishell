
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
