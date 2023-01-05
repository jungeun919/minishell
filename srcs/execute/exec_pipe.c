
#include "minishell.h"

void	make_pipe(t_exec_token token, t_env *env_list)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		error_exit("pipe error\n", 1);
	pid = fork();
	if (pid == -1)
		error_exit("fork error\n", 1);
	if (pid == 0)
		child_process(fd, token, env_list);
	waitpid(pid, NULL, WNOHANG);
	parent_process(fd);
}

void	child_process(int *fd, t_exec_token token, t_env *env_list)
{
	close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(fd[1]);

	set_redir(token.parser_token, env_list);

	if (is_builtin(&token))
		exec_builtin(&token, env_list);
	else
		run_execve_cmd(token.cmd, env_list);
}

void	parent_process(int *fd)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(fd[0]);
}
