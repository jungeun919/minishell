
#include "minishell.h"

void	set_redir(t_execute_unit *token, t_env *env_list)
{
	int	i;

	i = 0;
	while (token->in[i] != NULL)
	{
		if (ft_strncmp(token->in[i], "<<", 3) == 0)
			get_infile(token->in[i + 1], env_list);
		set_redir_in(token->in[i], token->in[i + 1]);
		i += 2;
	}
	i = 0;
	while (token->out[i] != NULL)
	{
		set_redir_out(token->out[i], token->out[i + 1]);
		i += 2;
	}
}

void	get_infile(char *limiter, t_env *env_list)
{
	int		fd;
	char	*line;

	fd = open(".here_doc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_exit("open error\n", 1);
	while (1)
	{
		line = readline("> ");
		if (line)
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
			{
				free(line);
				break ;
			}
			line = replace_env_heredoc(line, env_list);
			// printf("line : %s\n", line);
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		else
			break ;
	}
	close(fd);
}

char	*replace_env_heredoc(char *str, t_env *env_list)
{
	char	*key;
	char	*key_end;
	char	*temp;
	
	while (ft_strchr(str, '$'))
	{
		key = ft_strchr(str, '$'); // $의 인덱스
		key_end = key + 1; // 환경변수 끝 하나 뒤 인덱스 ("012$PATH 345") <- key = '$', key_end = ' '
		while (ft_isalnum(*key_end))
			key_end++;
		key = ft_substr(key, 1, (int)(key_end - key - 1)); // $다음 인덱스 부터 저장
		temp = join_env(str, get_env_value(env_list,key), key_end);
		free(str);
		str = temp;
		free(key);
	}
	return (str);
}

void	set_redir_in(char *redir_sign, char *filename)
{
	int	fd;

	fd = -1;
	if (ft_strncmp(redir_sign, "<", 2) == 0)
		fd = open(filename, O_RDONLY);
	else if (ft_strncmp(redir_sign, "<<", 3) == 0)
		fd = open(".here_doc_temp", O_RDONLY);
	if (fd == -1)
	{
		// printf("%s: no such file or directory\n", filename);
		error_exit("open error\n", 1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(fd);
}

void	set_redir_out(char *redir_sign, char *filename)
{
	int	fd;

	fd = -1;
	if (ft_strncmp(redir_sign, ">", 2) == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strncmp(redir_sign, ">>", 3) == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		// printf("%s: no such file or directory\n", filename);
		error_exit("open error\n", 1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(fd);
}
