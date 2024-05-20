/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/20 10:21:08 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// readline malloc's a new buffer every time.
int	sniff_line(t_data *data)
{
	data->line_read = readline("[I can't believe this is not shell] ");
	if (!data->line_read)
		return (0);
	line_parsing(data->line_read);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	return (1);
}

void		line_parsing(char *line)
{
	return ;
}
