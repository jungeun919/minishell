
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

void	set_echoctl_off(void) // ctrl+c not print
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	set_echoctl_on(void) // ctrl+c print
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}