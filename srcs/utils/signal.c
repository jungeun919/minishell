
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
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		exit(1);
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
