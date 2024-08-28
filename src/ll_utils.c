/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 09:30:58 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 11:40:49 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*free_node_content(t_env *node)
{
	free_null(node);
	return (NULL);
}

static t_env	*free_tmp_array(t_env *node)
{
	free_null(node->content);
	free_null(node);
	return (NULL);
}

static t_env	*free_node_key(t_env *node, char **array)
{
	free_null(node->content);
	free_null(node);
	free_array(array);
	return (NULL);
}

static t_env	*free_node_value(t_env *node, char **array)
{
	free_null(node->content);
	free_null(node->key);
	free_null(node);
	free_array(array);
	return (NULL);
}

t_env	*ft_listnew(void *content)
{
	t_env	*node;
	char	**tmp_array;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = ft_strdup(content);
	if (!node->content)
		return (free_node_content(node));
	tmp_array = ft_split(content, '=');
	if (!tmp_array)
		return (free_tmp_array(node));
	node->key = ft_strdup(tmp_array[0]);
	if (!node->key)
		return (free_node_key(node, tmp_array));
	node->value = ft_strdup(ft_strchr(content, '=') + 1);
	if (!node->value)
		return (free_node_value(node, tmp_array));
	node->next = NULL;
	free_array(tmp_array);
	return (node);
}
