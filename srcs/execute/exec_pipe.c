
#include "minishell.h"

void	exec_pipe(t_exec_token token, int i, pid_t *pids, int **fds, t_env *env_list, int len)
{
	// int	status;

	// if (pipe(fds[i]) == -1)
	// 	error_exit("pipe error\n", 1);
	pids[i] = fork();
	if (pids[i] == -1)
		error_exit("fork error\n", 1);
	if (pids[i] == 0)
		child_process(fds, i, token, env_list, len);
	// int	status;
	// wait(&status);
	// waitpid(pids[i], &status, 0);
	fprintf(stderr, "[%s] pid : %d\n", token.cmd[0], pids[i]);
}

void	child_process(int **fds, int i, t_exec_token token, t_env *env_list, int len)
{
	fprintf(stderr, "%s : fds[%d][read] : %d\n", token.cmd[0], i, fds[i][0]);
	fprintf(stderr, "%s : fds[%d][write] : %d\n", token.cmd[0], i, fds[i][1]);

	if (i != 0)
	{
		close(fds[i - 1][1]);
		dup2(fds[i - 1][0], STDIN_FILENO);
		close(fds[i - 1][0]);
	}

	if (i != len - 1)
	{
		close(fds[i][0]);
		dup2(fds[i][1], STDOUT_FILENO);
		close(fds[i][1]);
	}
	
	int j = 0;
	while (j < len)
	{
		close(fds[j][0]);
		close(fds[j][1]);
		j++;

	}


	// if (i == 0)
	// {
	// 	close(fds[i][0]);
	// 	dup2(fds[i][1], STDOUT_FILENO);
	// 	close(fds[i][1]);
	// }
	// else if ((i == len - 1) && (len != 1))
	// {
	// 	close(fds[i - 1][1]);
	// 	dup2(fds[i - 1][0], STDIN_FILENO);
	// 	close(fds[i - 1][0]);
	// }
	// else
	// {
	// 	close(fds[i - 1][1]);
	// 	dup2(fds[i - 1][0], STDIN_FILENO);
	// 	close(fds[i - 1][0]);
	// 	close(fds[i][0]);
	// 	dup2(fds[i][1], STDOUT_FILENO);
	// 	close(fds[i][1]);
	// }

	// int	j = 0;
	// while (j < len)
	// {
	// 	close(fds[j][0]);
	// 	close(fds[j][1]);
	// 	j++;
	// }


	set_redir(token.parser_token, env_list);

	if (is_builtin(&token))
		exec_builtin(&token, env_list);
	else
		run_execve_cmd(token.cmd, env_list);
}

// void	parent_process(int **fds, int i)
// {
// 	fprintf(stderr, "parent %d\n", i);
// 	// 읽기 닫기, 쓰기 지정 (stdout 설정)
// 	if (i != 1) //i != len - 1
// 	{
// 		close(fds[i][1]);
// 		if (dup2(fds[i][0], STDIN_FILENO) == -1)
// 			error_exit("dup2 error\n", 1);
// 		close(fds[i][0]);
// 		dup2(fds[i][0], fds)
// 	}
// }
