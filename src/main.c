/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 17:06:46 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_code = 0;

int	main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			**env_ll;
	int 			status;

	(void)argc;
	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!env_ll)
	{
		free_null(data);
		return (1);
	}
	initializer(data, env_ll, env);
	status = wow_loop(data, env_ll);
	super_free(data, env_ll);
	return (status);
}
