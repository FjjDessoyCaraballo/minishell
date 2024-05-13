/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 12:00:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		(void)argv;
		char *buf;
		
		while ((buf = readline("[My heart shell go on] ")) != NULL) {
			if (ft_strlen(buf) > 0) {
			add_history(buf);
			}

			// printf("[%s]\n", buf);

			// readline malloc's a new buffer every time.
			free(buf);
		}
	}
	return (0);
}
