/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:20:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 14:36:12 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**tokens_to_array(t_token *token_list)
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
        if (current->type == COMMAND || current->type == FLAG
		|| current->type == ARGUMENT)
            count++;
        current = current->next;
    }
	array = (char **)malloc((count + 1) * sizeof(char *));
    if (array == NULL)
		return NULL;
    current = token_list;
    while (current != NULL) 
	{
        if (current->type == COMMAND || current->type == FLAG
		|| current->type == ARGUMENT) 
		{
            array[index] = current->value;
            index++;
        }
        current = current->next;
    }
    array[index] = NULL;
    return (array);
}

