/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hannes <hrother@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:06:11 by hrother           #+#    #+#             */
/*   Updated: 2024/06/09 11:11:02 by hannes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char const *argv[], char *envp[])
{
	int		exit_status;
	t_evars	evars;

	(void)argc;
	(void)argv;
	evars.status = 0;
	evars.tty = isatty(STDIN_FILENO);
	evars.state = DEFAULT;
	if (evars.tty == -1)
		return (1);
	evars.envl = envp_to_list(envp);
	if (!evars.envl)
		return (1);
	exit_status = shell_loop(&evars);
	ft_lstclear(&evars.envl, free_env);
	if (evars.tty && evars.state != CHILD)
		ft_putendl_fd("exit", STDERR_FILENO);
	return (exit_status);
}
