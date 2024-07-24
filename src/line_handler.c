/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/07/24 09:47:48 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sniff_line(t_data *data)
{
	data->line_read = readline("\e[45m[I can't believe it's not shell]\e[0m ");
	// dprintf(2, "contents of line_Read:%s\n", data->line_read);
	if (!data->line_read)
		return (0);
	line_tokenization(data);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	return (1);
}
