/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:38:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/14 11:05:48 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// node is getting a copy of env in a linked list
void	copy_env(t_env **env_ll, char **env)
{
	t_env	*tmp;
	int		i;
	
	i = 0;
	if (*env_ll == NULL)
		*env_ll = ft_listnew(env[i++]);
	tmp = (*env_ll);
	while (env[i])
		ft_listadd_back(env_ll, ft_listnew(env[i++]));
	(*env_ll) = tmp;
	print_env(env_ll);
}

void	initializer(t_data *data, t_env **env_ll, char **env)
{
	(void)data;
	copy_env(env_ll, env);
}

void	print_env(t_env **env_ll)
{
	t_env	*tmp;

	tmp = *env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
}