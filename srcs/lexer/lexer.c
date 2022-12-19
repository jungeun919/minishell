
#include "../../includes/minishell.h"

t_list	*lexer(char *str)
{
	static t_list *lexer_tokens;
	int i;

	if (str[0] == '\0')
		return lexer_tokens;
	else if (str[0] == '|')
	{
		ft_lstadd_back(&lexer_tokens, ft_lstnew(ft_strdup("|")));
		lexer(str+1);
	}
	else if (str[0] == '\"' || str[0] == '\'')
	{
		i = 1;
		while (str[i] != '\0' && str[i] != str[0]) // 홀수 따옴표 갯수는 다음 단계에서 처리
			i++;
		ft_lstadd_back(&lexer_tokens, ft_lstnew(ft_substr(str, 0, i+1)));
		lexer(str+i+1);
	}
	else if (str[0] == '<' || str[0] == '>'){
		if (str[1] == str[0]){
			ft_lstadd_back(&lexer_tokens, ft_lstnew(ft_substr(str, 0, 2)));
			lexer(str+2);
		}
		else
		{
			ft_lstadd_back(&lexer_tokens, ft_lstnew(ft_substr(str, 0, 1)));
			lexer(str+1);
		}
	}
	else if (ft_strchr("\t\n\v\f\r ", str[0]))
	{
		lexer(str+1);
	}
	else {
		i = 0;
		while (!ft_strchr("|\'\"><\t\n\v\f\r ", str[i]))
			i++;
		ft_lstadd_back(&lexer_tokens, ft_lstnew(ft_substr(str, 0, i)));
		lexer(str+i);
	}
	return lexer_tokens;

	// | 처리
	// " ' 처리
	// 		-> " or ' 이 나오면 다음 ", ' 이 나올때 까지 뒤로 간후 
	// << >> < > 처리
}

int main() {
	t_list *lexer_token = lexer("  cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2 \"$asd 43 rew");
	lexer_token ++;
	lexer_token --;
}