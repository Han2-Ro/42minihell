/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:47:08 by hannes            #+#    #+#             */
/*   Updated: 2024/05/27 17:03:20 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_nflag(const char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

int	builtin_echo(const t_cmd *cmd)
{
	bool	n_flag;
	int		i;

	i = 1;
	while (cmd->args[i] && is_nflag(cmd->args[i]))
		i++;
	n_flag = (i > 1);
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], cmd->fd_out);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", cmd->fd_out);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", cmd->fd_out);
	return (SUCCESS);
}
