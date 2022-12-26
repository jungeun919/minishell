
#include "minishell.h"

void	setting_signal()
{
	signal(SIGINT, sig_handler); // ctrl + c
	signal(SIGQUIT, SIG_IGN); // ctrl + /
}

void	sig_handler(int signal)
{
	// printf("signal: %d\n", signal);
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}
