/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:20:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 13:41:03 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * ttda - token to array w/ delimiter
 * 
 * USAGE: give the token and the delimiter. Delimiters: types of tokens.
 * Use '0' if you don't want to have a delimiter.
 * 
 * Types of tokens: COMMANDS, FLAGS, ARGUMENTS, REDIRECTS.
 */
/**
 * COMMAND LINE TO ARRAY
 * This function takes all tokens and organize them into elements of an array
 * so that we can distribute instructions to each child process individually.
 * 
 * E.g.:
 * *************** IN COMMAND LINE ********************
 * %> ls -la | grep Makefile | cat > outfile
 * ****************************************************
 * The array will take the instructions and organize them as follows:
 * pipe_array[0] = "ls -la"
 * pipe_array[1] = "grep Makefile"
 * pipe_array[2] = "cat > outfile"
 * 
 * ***NOTE: I dont think we need malloc checks after strdup, as its taken care of inside the function
 * 			that should save a few lines at least***
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
		{
			break ;
		}
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
		if ((current->type == COMMAND || current->type == FLAG
			|| current->type == ARGUMENT) && (current->type != PIPE))
			array[index++] = current->value;
		current = current->next;
	}
	array[index] = NULL;
	return (array);
}
