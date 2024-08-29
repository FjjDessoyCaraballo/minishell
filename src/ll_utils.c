/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:54:37 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 13:54:39 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(env_ll->content, "HOME=", 5))
			return (env_ll->content + 5);
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	return (NULL);
}

void	ft_listadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst)
	{
		if (*lst)
		{
			last = ft_list_last(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

t_env	*ft_list_last(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}
