/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:59:50 by aprevrha          #+#    #+#             */
/*   Updated: 2024/05/30 18:08:11 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_readline(char *prompt, int tty)
{
	char	*line;

	if (tty)
		line = readline(prompt);
	else
	{
		line = read_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	log_msg(DEBUG, "Inputed line: %s", line);
	return (line);
}

int	process_line(char *line, t_evars *evars)
{
	t_list	*token_lst;
	t_list	*cmd_lst;

	token_lst = lexer(line, &(evars->status));
	if (expand_token_list(&token_lst, *evars) != SUCCESS)
		return (ft_lstclear(&token_lst, free_token), 2);
	if (!token_lst)
		return (evars->status);
	if (!token_lst)
		log_msg(DEBUG, "Lex: token list null");
	cmd_lst = parse(token_lst);
	if (!cmd_lst)
		return (ft_lstclear(&token_lst, free_token), 2);
	ft_lstiter(cmd_lst, print_cmd);
	exec_cmd_list(cmd_lst, evars);
	log_msg(DEBUG, "status: %i", evars->status);
	ft_lstclear(&token_lst, free_token);
	return (evars->status);
}

char	*get_line(t_evars *evars)
{
	char	*line;
	char	*prompt;

	prompt = NULL;
	if (evars->tty)
		prompt = get_prompt(evars);
	if (prompt)
	{
		line = ft_readline(prompt, evars->tty);
		free(prompt);
	}
	else
		line = ft_readline(PROMPT, evars->tty);
	return (line);
}

int	shell_loop(t_evars *evars)
{
	char	*line;

	while (1)
	{
		idle_signals();
		line = get_line(evars);
		if (!line)
			break ;
		if (evars->tty && *line)
			add_history(line);
		if (g_signal)
			evars->status = g_signal + 128;
		g_signal = 0;
		evars->status = process_line(line, evars);
		free(line);
		if (evars->state == EXIT || evars->state == CHILD)
			break ;
	}
	rl_clear_history();
	return (evars->status);
}
