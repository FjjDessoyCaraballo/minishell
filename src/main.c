/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 15:57:26 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			*env_ll;
	int		exec_status;
	
	(void)argv;
	// data = malloc(sizeof(t_data));
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		exit(1); // might need to specify error for this
	env_ll = NULL;
	initializer(data, &env_ll, env);
	exec_status = 0;
	if (argc == 1)
	{
		while (666)
		{
			exec_status = sniff_line(data);
			if (exec_status == NULL_LINE)
			{
				printf("exit\n");
				break ;
			}
			else if (exec_status == 0)
				execution(data, &env_ll);
			free_token(data->token);
		}
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (data->status);
}
