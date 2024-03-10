/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_structs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:12:46 by hrother           #+#    #+#             */
/*   Updated: 2024/03/10 22:29:52 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	print_cmd(void *command)
{
	t_cmd		*cmd;
	int			i;
	t_log_level	level;

	level = INFO;
	cmd = (t_cmd *)command;
	log_msg(level, "cmd: %s", cmd->bin);
	log_msg(level, "pid: %d", cmd->pid);
	log_msg(level, "args:");
	i = 0;
	while (cmd->args[i] != NULL)
	{
		log_msg(level, "  %s", cmd->args[i]);
		i++;
	}
	log_msg(level, "fd_in: %d, fd_out: %d", cmd->fd_in, cmd->fd_out);
	ft_lstiter(cmd->redirects, print_token);
}

void	print_token(void *token)
{
	t_token	*tkn;

	tkn = (t_token *)token;
	log_msg(INFO, "type: %i value: %s", tkn->type, tkn->value);
}

void	print_list(t_list *lst)
{
	while (lst != NULL)
	{
		print_cmd((t_cmd *)lst->content);
		lst = lst->next;
	}
}