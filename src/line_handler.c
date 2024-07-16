/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/23 13:37:45 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sniff_line(t_data *data)
{
	data->line_read = readline("🤓: ");
	if (!data->line_read)
		return (0);
	line_tokenization(data);
	// line_parsing(data->line_read); // LEO'S PART
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	return (1);
}
