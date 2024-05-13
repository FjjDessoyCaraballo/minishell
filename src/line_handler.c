/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 13:52:07 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// readline malloc's a new buffer every time.
void	sniff_line(t_data *data)
{
	data->line_read = readline("[My heart shell go on] ");
	if (!data->line_read)
		error_exit(69);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
}