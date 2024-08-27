/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 11:58:28 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	t_env	**env_ll;
	int		status;

	if (argc != 1)
		return (err_msg(NULL, ERR_ARG, 1));
	env = add_shell_lvl(env);
	(void)argv;
	status = 0;
	if (!env || !*env)
		return (1);
	data = ft_calloc(1, sizeof(t_data));
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!env_ll || !data)
	{
		if (!data)
			free_null(data);
		return (1);
	}
	initializer(data, env_ll, env);
	status = wow_loop(data, env_ll);
	super_free(data, env_ll, env);
	return (status);
}
