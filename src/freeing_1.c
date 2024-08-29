/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:54:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 13:54:08 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ll(t_env *env_ll)
{
	t_env	*tmp;

	while (env_ll != NULL)
	{
		tmp = env_ll;
		free(env_ll->key);
		free(env_ll->value);
		free(env_ll->content);
		env_ll = env_ll->next;
		free(tmp);
		tmp = NULL;
	}
}

void	cleanup_node(t_env *node, char **tmp_array)
{
	if (tmp_array)
		free_array(tmp_array);
	if (node)
	{
		free_null(node->key);
		free_null(node->value);
		free_null(node->content);
		free(node);
	}
}
