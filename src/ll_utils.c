/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 09:30:58 by fdessoy-          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/05/21 11:51:07 by fdessoy-         ###   ########.fr       */
=======
/*   Updated: 2024/08/02 12:50:29 by lstorey          ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_listnew(void *content)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

char	*get_home(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(env_ll->content, "HOME=", 5))
			return (env_ll->content + 5);
<<<<<<< HEAD
		env_ll = env_ll->next;		
=======
		env_ll = env_ll->next;
>>>>>>> main
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

void	free_ll(t_env *env_ll)
{
	t_env	*tmp;

	while (env_ll != NULL)
	{
		tmp = env_ll;
		env_ll = env_ll->next;
		free(tmp);
	}
}
