/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/07/25 18:46:33 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
    if (sig == SIGINT)
    {
	    //printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
        g_exit_code = 130;
	    rl_on_new_line();
	    rl_replace_line("", 0);
	    rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        //printf("\nCaught signal %d (Ctrl-\\).Clearing history and exiting...\n", sig);
        g_exit_code = 131;
        clear_history();
        signal(sig, SIG_DFL);  
        kill(getpid(), sig);
    }
}
