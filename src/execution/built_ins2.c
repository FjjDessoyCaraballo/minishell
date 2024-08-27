/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy <fdessoy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 21:12:16 by fdessoy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	alphabetical_printer(char **env_array)
{
	char	c;
	int		i;

	i = 0;
	c = 32;
	while (c < 127)
	{
		while (env_array[i])
		{
			if (env_array[i][0] == c)
			{
				ft_printf("declare -x ");
				ft_printf("%s\n", env_array[i]);
			}
			i++;
		}
		i = 0;
		c++;
	}
}

int	print_export(t_env **env_ll)
{
	char	**env_array;

	if (!env_ll || !*env_ll)
		return (SUCCESS);
	env_array = env_arr_updater(env_ll);
	if (!env_array)
		return (SUCCESS);
	alphabetical_printer(env_array);
	free_array(env_array);
	return (SUCCESS);
}
