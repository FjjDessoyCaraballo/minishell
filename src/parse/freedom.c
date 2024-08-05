/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freedom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:19:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 13:19:50 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_my_boi(char **paths)
{
	int i;
	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
