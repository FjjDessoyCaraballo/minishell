/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:03:43 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 16:18:43 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_null(void *ptr)
{
	free(ptr);
	ptr = NULL;
	return ;
}

void	malloc_check_message(void *ptr)
{
	if (!ptr)
		exit(err_msg(NULL, MALLOC, -1));
	else
		return ;
}

void	super_free(t_data *data, t_env **env_ll)
{
	free_null(data->fin_tok);
	free_array(data->binary_paths);
	free_all_ll(env_ll);
	free(data);
}