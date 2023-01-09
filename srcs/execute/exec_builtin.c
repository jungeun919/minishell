
#include "minishell.h"

int	is_builtin(t_exec_token *token)
{
	char	*cmd;

	cmd = token->parser_token->cmd->content;
	if (ft_strncmp(cmd, "env", 4) == 0 || \
		ft_strncmp(cmd, "export", 7) == 0 || \
		ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else
		return (0);
}

void	exec_builtin(t_exec_token *token, t_env *env_list)
{
	char	*cmd;
	int		status;

	cmd = token->cmd[0];
	status = 0;
	if (ft_strncmp(cmd, "env", 4) == 0)
		status = ft_env(token->cmd, env_list);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		status = ft_export(token->cmd, env_list);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		status = ft_unset(token->cmd, env_list);
	exit(status);
}
