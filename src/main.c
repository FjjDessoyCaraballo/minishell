/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 15:12:38 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_data	data;
	
	(void)argv;
	initializer(&data, env);
	if (argc == 1)
	{
		while (666)
			sniff_line(&data);
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	return (0);
}
