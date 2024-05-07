/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprevrha <aprevrha@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:59:50 by aprevrha          #+#    #+#             */
/*   Updated: 2024/05/06 20:40:51 by aprevrha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/ioctl.h>

int		g_sig = 0;

// log_msg(INFO, "I S: %i", sig_num);
void	idle_signal_handler(int sig_num)
{
	g_sig = sig_num;
	if (sig_num == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

// log_msg(INFO, "A S: %i", sig_num);
void	active_signal_handler(int sig_num)
{
	g_sig = sig_num;
	ft_putchar_fd('\n', STDIN_FILENO);
}

int	idle_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = idle_signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction");
		log_msg(ERROR, "failed to register signal SIGINT");
		return (1);
	}
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction");
		log_msg(ERROR, "failed to register signal SIGQUIT");
		return (1);
	}
	return (0);
}

int	active_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = active_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		log_msg(ERROR, "failed to register signal SIGINT");
		return (1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		log_msg(ERROR, "failed to register signal SIGQUIT");
		return (1);
	}
	return (0);
}
