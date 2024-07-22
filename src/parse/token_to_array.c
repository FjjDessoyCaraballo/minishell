/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:20:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 16:21:22 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * ttda - token to array w/ delimiter
 * USAGE: give the token and the delimiter. Delimiters: types of tokens.
 * Use '0' if you don't want to have a delimiter.
 * Types of tokens: COMMANDS, FLAGS, ARGUMENTS, REDIRECTS.
 */
char	**ttad(t_token *token_list, t_type delimiter)
{
	int		count;
	int		index;
	char	**array;
	t_token	*current;

	count = 0;
	index = 0;
	current = token_list;
	while (current != NULL)
	{
		if (current->type == delimiter)
			break ;
		if (current->type == COMMAND || current->type == FLAG
			|| current->type == ARGUMENT)
			count++;
		current = current->next;
	}
	array = (char **)malloc(sizeof(char *) * (count + 1));
    if (array == NULL)
		return NULL;
	current = token_list;
	while (current != NULL && current->type != delimiter)
	{
		if (current->type == COMMAND || current->type == FLAG
			|| current->type == ARGUMENT)
			array[index++] = current->value;
		current = current->next;
	}
	array[index] = NULL;
	return (array);
}
