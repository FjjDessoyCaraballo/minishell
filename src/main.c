/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/14 10:45:22 by fdessoy-         ###   ########.fr       */
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
	if (argc == 1)
	{
		while (666)
			sniff_line(&data);
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (0);
}
