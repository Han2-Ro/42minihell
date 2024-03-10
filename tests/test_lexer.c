/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprevrha <aprevrha@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:19:38 by hrother           #+#    #+#             */
/*   Updated: 2024/03/08 18:59:54 by aprevrha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "utils/run_test.c"

// int check(char *line, )

void	print_token_new(void *p_tkn)
{
	t_token	*token;

	token = ((t_token *)p_tkn);
	char *names[] = {
		"NOTDEF",
		"CMD",
		"ARG",
		"R_IN",
		"R_OUT",
		"R_APPEND",
		"R_HEREDOC",
		"PIPE",
	};
	if (token->type < 0 || token->type > 7)
		log_msg(WARNING, "type = %i: Type out of range!\n", token->type);
	log_msg(DEBUG, "%i: %s\n", token->type, names[token->type]);
	log_msg(DEBUG, "value = \"%s\"\n", token->value);
}

int	test_lexer(char **envp)
{
	t_list	*token_lst;
	char	*line;

	log_msg(WARNING, "This test needs manual inspection of the output");
	(void)envp;
	line = "     ls -l>out1 -a | << END grep 123 >> out1 >out2||";
	token_lst = lexer(line);
	ft_lstiter(token_lst, print_token_new);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test lexer", test_lexer, envp);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}