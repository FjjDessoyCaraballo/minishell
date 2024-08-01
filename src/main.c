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
	static int		status;
	
	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	if (argc == 1)
	{
		env_ll = NULL;
		initializer(data, &env_ll, env);
		while (666)
		{
			status = sniff_line(data);
			if (status == NULL_LINE)
			{
				printf("exit\n");
				break ;
			}
			else
				execution(data, &env_ll);
			free_token(data->token);
			data->token = NULL;
		}
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	free(data);
	return (0);
}
