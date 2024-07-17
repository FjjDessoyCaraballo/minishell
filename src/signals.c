/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/07/15 12:59:58 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
    if (sig == SIGINT)
    {
	    printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	    rl_on_new_line();
	    rl_replace_line("", 0);
	    rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        printf("\nCaught signal %d (Ctrl-\\).Clearing history and exiting...\n", sig);
        clear_history();
        signal(sig, SIG_DFL);  
        kill(getpid(), sig);
    }
}
