/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 21:42:30 by walnaimi         ###   ########.fr       */
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
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!data  || !env_ll)
		return (1);
	initializer(data, env_ll, env);
	while (666)
	{
		status = sniff_line(data);
		if (status == NULL_LINE)
		{
			printf("exit\n");
			break ;
		}
		else if (status != 963)
			execution(data, env_ll);
		free_gang(data);
	}
	super_free(data, env_ll);
	return (0);
}
