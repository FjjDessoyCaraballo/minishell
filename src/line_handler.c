/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/19 17:36:49 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sniff_line(t_data *data)
{
	data->line_read = \
	readline("\e[45m[I can't believe this is not shell]\e[0m ");
	if (!data->line_read)
		return (0);
	line_tokenization(data);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	return (1);
}
