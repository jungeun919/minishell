
#include "minishell.h"

int	init_exec_info(pid_t **pids, int ***fds, int len)
{
	int	i;

	*pids = (pid_t *)malloc(sizeof(pid_t) * len);
	*fds = (int **)malloc(sizeof(int *) * len);
	if (*pids == NULL || *fds == NULL)
		return (free_init_exec_info(pids, fds, -1));
	i = 0;
	while (i < len)
	{
		(*fds)[i] = (int *)malloc(sizeof(int) * 2);
		if ((*fds)[i] == NULL)
			return (free_init_exec_info(pids, fds, i));
		i++;
	}
	i = 0;
	while (i < len)
	{
		if (pipe((*fds)[i]) == -1)
			return (free_init_exec_info(pids, fds, len - 1));
		i++;
	}
	return (0);
}

void	close_all_fds(int **fds, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void	wait_all_childs(int len)
{
	int	i;
	int	status;

	i = 0;
	while (i < len)
	{
		if (wait(&status) < 0)
		{
			status = 1;
			break ;
		}
	}
}

void	exec_pipe(t_exec_token token, int i, pid_t *pids, int **fds, t_env *env_list, int len)
{
	pids[i] = fork();
	if (pids[i] == -1)
		error_exit("fork error\n", 1);
	if (pids[i] == 0)
		child_process(fds, i, token, env_list, len);	
}

void	child_process(int **fds, int i, t_exec_token token, t_env *env_list, int len)
{
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
	i = 0;
	while (i < len)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
	set_redir(token.parser_token, env_list);
	if (is_builtin(&token))
		exec_builtin(&token, env_list);
	else
		run_execve_cmd(token.cmd, env_list);
	exit(g_info.exit_status);
}
