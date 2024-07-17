/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/16 13:56:20 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sniff_line(t_data *data)
{
<<<<<<< HEAD
	data->line_read = readline("🤓: ");
=======
	data->line_read = \
	readline("\e[45m[I can't believe this is not shell]\e[0m ");
>>>>>>> b6a25bb71a7cb3350356b6d3c250c8971cb53451
	if (!data->line_read)
		return (0);
	line_tokenization(data);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	return (1);
}
