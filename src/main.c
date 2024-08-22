/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 13:26:11 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_code = 0;

int	main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			**env_ll;
	int 			status;

	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!data  || !env_ll)
		return (1);
	if (argc == 1)
	{
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
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	free_null(data->fin_tok);
	free_array(data->binary_paths);
	free_ll((*env_ll));
	free_null(env_ll);
	free(data);
	return (0);
}
