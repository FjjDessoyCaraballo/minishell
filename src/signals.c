/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/07/30 10:20:18 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
    if (sig == SIGINT)
    {
	    printf("\n");
	    rl_on_new_line();
	    rl_replace_line("", 0);
	    rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        printf("\n");
        clear_history();
        signal(sig, SIG_DFL);  
        kill(getpid(), sig);
    }
}
