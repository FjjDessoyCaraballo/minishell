/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:38:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 15:14:14 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// james is getting a copy of env in a linked list
int	copy_env(char **env)
{
	t_list	*node;
	static int index = 0;

	node = NULL;
	while (env[index])
	{
		node->content = ft_lstnew(env[index]);
		node->next = node;
		index++;
	}
	if (!node)
		return (0);
	return (1);
}

void	initializer(t_data *data, char **env)
{
	t_list *node = NULL;
	
	(void)data;
	if(copy_env(env) == 0)
		error_exit(1);
	while (node->next != NULL)
		printf("%s\n", node->content);
}