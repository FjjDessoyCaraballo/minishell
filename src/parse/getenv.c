/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:53:13 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 13:53:14 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Retrieves the value associated with a 
 * given key from the environment linked list.
 * 
 * @param key The key to search for in the environment linked list.
 * @param env_ll The head of the environment linked list.
 * 
 * @return A pointer to the value associated
 * with the key if found, NULL otherwise.
 */

char	*ft_getenv(char *key, t_env *env_ll)
{
	t_env	*env;
	size_t	key_len;

	env = env_ll;
	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->content, key, key_len) == 0
			&& env->content[key_len] == '=')
		{
			free_null(key);
			return (env->content + key_len + 1);
		}
		env = env->next;
	}
	free_null(key);
	return (NULL);
}
