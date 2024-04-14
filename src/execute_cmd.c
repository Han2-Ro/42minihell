/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:21:59 by hrother           #+#    #+#             */
/*   Updated: 2024/04/14 18:00:25 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>

extern int	g_status;

void	log_file_error(const char *filename)
{
	log_msg(ERROR, "%s: %s", filename, strerror(errno));
}

int	setup_pipes(t_list *cmd_list)
{
	t_list	*current_cmd;
	int		pipe_fds[2];

	current_cmd = cmd_list;
	while (current_cmd != NULL && current_cmd->next != NULL)
	{
		if (pipe(pipe_fds) != SUCCESS)
			return (log_msg(ERROR, "pipe: %s", strerror(errno)), FAILURE);
		((t_cmd *)current_cmd->content)->fd_out = pipe_fds[1];
		((t_cmd *)current_cmd->next->content)->fd_in = pipe_fds[0];
		current_cmd = current_cmd->next;
	}
	return (SUCCESS);
}

int	wait_pids(t_list *cmd_list)
{
	t_list	*tmp;

	tmp = cmd_list;
	while (tmp != NULL)
	{
		log_msg(DEBUG, "waiting for pid: %d", ((t_cmd *)tmp->content)->pid);
		if (((t_cmd *)tmp->content)->pid > 0)
		{
			waitpid(((t_cmd *)tmp->content)->pid,
				&((t_cmd *)tmp->content)->status, 0);
			((t_cmd *)tmp->content)->status = WEXITSTATUS(((t_cmd *)tmp->content)->status);
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}

void	close_fds(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
}

int	exec_cmd(t_cmd *cmd, t_list *cmd_list, t_list **envlst)
{
	char	**envp_array;

	if (cmd->fd_in < 0 || cmd->fd_out < 0)
		return (FAILURE);
	if (is_builtin(cmd))
	{
		cmd->status = exec_builtin(cmd, envlst);
		return (cmd->status);
	}
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (log_msg(ERROR, "fork: %s", strerror(errno)), FAILURE);
	if (cmd->pid > 0)
		return (SUCCESS);
	if (cmd->fd_in > 2)
		dup2(cmd->fd_in, STDIN_FILENO);
	if (cmd->fd_out > 2)
		dup2(cmd->fd_out, STDOUT_FILENO);
	cmd->bin = path_to_bin(cmd->bin, *envlst);
	envp_array = envlst_to_envp(*envlst);
	ft_lstclear(envlst, free_env);
	ft_lstiter(cmd_list, close_fds);
	if (envp_array == NULL)
		(free(cmd->bin), free_cmd(cmd), exit(1));
	log_msg(INFO, "executing %s", cmd->bin);
	if (access(cmd->bin, X_OK) == 0)
		execve(cmd->bin, cmd->args, envp_array);
	exit(127);
	return (EXIT_FAILURE);
}

int	exec_cmd_list(t_list *cmd_list, t_list **envp, int *status)
{
	t_list	*current_cmd;

	if (setup_pipes(cmd_list) != EXIT_SUCCESS
		|| redirs_to_fds(cmd_list) != EXIT_SUCCESS)
		return (ft_lstclear(&cmd_list, free_cmd), EXIT_FAILURE);
	current_cmd = cmd_list;
	while (current_cmd != NULL)
	{
		exec_cmd((t_cmd *)current_cmd->content, cmd_list, envp);
		current_cmd = current_cmd->next;
	}
	ft_lstiter(cmd_list, close_fds);
	wait_pids(cmd_list);
	*status = ((t_cmd *)ft_lstlast(cmd_list)->content)->status;
	ft_lstclear(&cmd_list, free_cmd);
	return (EXIT_SUCCESS);
}
