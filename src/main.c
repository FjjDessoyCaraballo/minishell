/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/15 14:18:13 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_data	data;
	t_env	*env_ll;
	
	(void)argv;
	env_ll = NULL;
	initializer(&data, &env_ll, env);
	// ft_printf("%s", data.bin);
	if (argc == 1)
	{
		while (666)
		{
			if (sniff_line(&data) == 0)
			{
				printf("exit\n");
				break ;
			}
			built_ins(&data, env_ll);
		}
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (0);
}
