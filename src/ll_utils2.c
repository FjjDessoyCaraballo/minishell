/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 11:35:09 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 17:02:42 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* should probably not use getenv() here */
void    env_arr_updater(t_data *data, t_env **env_ll)
{
    int     i;
	int		size;
    t_env   *tmp;
    
    i = 0;
	tmp = (*env_ll);
	size = ll_size(env_ll);
	data->env = malloc(sizeof(char **) * size + 1);
	if (!data->env)
	{
		perror("Failed to allocate memory for data->env\n");
		return ;
	}
	while (i < size)
	{
		data->env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
        i++;
    }
    tmp = NULL;
}

int	ll_size(t_env **env_ll)
{
	t_env	*tmp;
	int		i;

	if (!env_ll || !*env_ll)
		return (0);
	i = 0;
	tmp = *env_ll;
	while (tmp->next != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	tmp = NULL;
	return (i);
}