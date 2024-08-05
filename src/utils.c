/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:23:58 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 10:24:13 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* These functions are general utils used throughout the code */

/**
 * Frees the shell most used data allocators.
 * USAGE: pass the function the arguments it asks, otherwhise use NULL.
 * data: main data struct for the shell;
 * path: defined and allocated in the execution part;
 * env: our linked list that works as our environment pointer;
 * command_array: variable linked to the execution of single/multiple processes.
 */
void	free_data(t_data *data, char *path, t_env **env, char **command_array)
{
	if (data->binary_paths)
		free_array(data->binary_paths);
	if (data->env)
		free_array(data->env);
	if (env)
		free_ll(*env);
	if (command_array)
		free_array(command_array);
	if (path)
	{
		free(path);
		path = NULL;
	}
}

void	free_token(t_token *token)
{
	t_token	*tmp;
	
	while (token != NULL)
	{
		tmp = token;
		token = token->next;
		free(tmp);
		tmp = NULL;
	}
	free(token);
	token = NULL;
}
