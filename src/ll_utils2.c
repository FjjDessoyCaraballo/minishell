/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 11:35:09 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 15:58:42 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* should probably not use getenv() here */
void    env_arr_updater(t_data *data, t_env *env_ll)
{
    int     i;
    t_env   *tmp;
    
    i = 0;
    tmp = env_ll;
    while (tmp->next != NULL)
        i++;
	tmp = env_ll;
	malloc()
    while (tmp->next != NULL)
    {
        printf("tmp->content: %s\n", tmp->content);
        data->env[i] = tmp->content;
        i++;
        tmp = tmp->next;
    }
    tmp = NULL;
}