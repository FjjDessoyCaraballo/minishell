/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 13:49:55 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 14:43:50 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_exit(int num)
{
	if (num == 1)
		ft_putstr_fd(ERR, 2);
	else if (num == 69)
		ft_putstr_fd(EXIT, 2);
	exit(1);
}

// // this is the error handler for allocations later
// void	error_alloc(char **array, int num)