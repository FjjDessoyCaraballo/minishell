/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:54:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 13:54:53 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_exit_code == HEREDOC_SIG)
			handler(SIGQUIT);
		else if (g_exit_code == EXEC_SIG)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sig == SIGQUIT)
	{
		printf("\n");
		clear_history();
		signal(sig, SIG_DFL);
		kill(getpid(), sig);
	}
}
