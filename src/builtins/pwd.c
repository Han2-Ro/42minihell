/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:17:44 by hrother           #+#    #+#             */
/*   Updated: 2024/04/30 18:03:22 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// TODO: consider using own envp list instead of getcwd
int	builtin_pwd(const t_cmd *cmd, t_list **envp)
{
	char	*cwd;

	(void)envp;
	log_msg(DEBUG, "executing builtin_pwd");
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		log_msg(ERROR, "pwd: %s", strerror(errno));
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cwd, cmd->fd_out);
	free(cwd);
	return (EXIT_SUCCESS);
}
