/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:21:59 by hrother           #+#    #+#             */
/*   Updated: 2024/05/14 16:43:55 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execution.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void	close_fds(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
}

int	setup_fds(t_cmd *cmd)
{
	bool	success;

	success = true;
	if (cmd->fd_in > 2)
		success = dup2(cmd->fd_in, STDIN_FILENO) >= 0;
	if (cmd->fd_out > 2)
		success &= dup2(cmd->fd_out, STDOUT_FILENO) >= 0;
	if (success)
		return (EXIT_SUCCESS);
	log_msg(ERROR, "dup2: %s", strerror(errno));
	return (EXIT_FAILURE);
}

int	try_exec_path(t_cmd *cmd, char **envp_array)
{
	struct stat	s_pstat;

	if (cmd->bin == NULL)
		return (EXIT_MASK | 127);
	if (stat(cmd->bin, &s_pstat) == -1)
		log_msg(ERROR, strerror(errno));
	else if (S_ISDIR(s_pstat.st_mode))
	{
		log_msg(ERROR, "Is a Directory");
		return (EXIT_MASK | 126);
	}
	if (cmd->bin && access(cmd->bin, X_OK) == 0)
	{
		execve(cmd->bin, cmd->args, envp_array);
		log_msg(ERROR, "%s: %s", cmd->bin, strerror(errno));
		return (EXIT_MASK | 126);
	}
	return (EXIT_MASK | 127);
}

int	exec_cmd(t_cmd *cmd, t_list *cmd_list, t_evars *evars, char **envp_array)
{
	if (cmd->fd_in < 0 || cmd->fd_out < 0 || cmd->args[0] == NULL)
		return (EXIT_FAILURE);
	if (is_builtin(cmd))
	{
		cmd->status = exec_builtin(cmd, &evars->envl, evars->status);
		return (cmd->status);
	}
	cmd->bin = path_to_bin(cmd->args[0], evars->envl);
	if (cmd->bin == NULL)
		(cmd->status = 127);
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (log_msg(ERROR, "fork: %s", strerror(errno)), EXIT_FAILURE);
	if (cmd->pid > 0)
		return (EXIT_SUCCESS);
	setup_fds(cmd);
	ft_lstiter(cmd_list, close_fds);
	log_msg(DEBUG, "executing %s", cmd->bin);
	cmd->status = try_exec_path(cmd, envp_array);
	cmd->status = EXIT_MASK | cmd->status;
	return (EXIT_FAILURE);
}
