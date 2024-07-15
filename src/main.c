/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 16:33:19 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_data	*data;
	t_env	*env_ll;
	
	(void)argv;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		exit(1); // might need to specify error for this
	env_ll = NULL;
	
	initializer(data, &env_ll, env);
	if (argc == 1)
	{
		while (666)
		{
			if (sniff_line(data) == 0)
			{
				printf("exit\n");
				break ;
			}
			execution(data, &env_ll);
		}
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (data->status);
}
