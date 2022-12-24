
#include "../../includes/minishell.h"

void lexer(char *str, t_list **lexer_tokens)
{
	int		i;
	char	*temp;
	t_list	*new;

	i = 0;
	if (str[0] == '\0')
		return ;
	else if (str[0] == '\"' || str[0] == '\'')
		while (str[++i] != '\0' && str[i] != str[0])
			;
	else if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		i++;
	else if (ft_strchr("\t\n\v\f\r ", str[0]))
		return lexer(str+1, lexer_tokens);
	else if (str[0] != '<' && str[0] != '>' && str[0] != '|')
		while (!ft_strchr("|\'\"><\t\n\v\f\r ", str[i + 1]))
			i++;
	temp = ft_substr(str, 0, i+1);
	new = ft_lstnew(temp);
	if (temp == NULL || new == NULL)
		ft_lstclear(lexer_tokens, free);
	ft_lstadd_back(lexer_tokens, new);
	if (str[i] == '\0')
		return ;
	lexer(str+i+1, lexer_tokens);
}

int main() {
	t_list *lexer_token = NULL;
	// lexer("  cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2 ", &lexer_token);
	lexer("\"asdf\'fasd  asdf \'\"asdfsdf", &lexer_token);
	t_list *temp = lexer_token;
	int node_index = 0;
	while (temp != NULL)
	{
		printf("%2d 번째 노드 : [%s]\n", node_index++, temp->content);
		temp = temp->next;
	}
}