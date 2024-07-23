/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:41:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/23 10:45:56 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * This function takes care of executing commands with no piping.
 * USAGE: %> cmd -flag argument OR %> cmd argument -flag
 */
int	single_execution(t_data *data, t_token *token, t_env **env_ll)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		free_ll((*env_ll));
		free_data(data, NULL, env_ll, NULL);
		perror("fork");
		data->status = -1;
		return (-1);
	}
	else if (pid == 0)
		single_child(data, token, env_ll);
	else
		status = single_parent(pid, status);
	return (status);
}

void	single_child(t_data *data, t_token *token, t_env **env_ll)
{
	char	**command_array;
	char	**env;
	char	*path;
	
	command_array = ttad(token, 0);
	path = ft_strsjoin(token->path, token->value, '/');
	env = env_arr_updater(env_ll);
	if (execve(path, command_array, env) == -1)
	{
		free_array(env);
		free_data(data, path, env_ll, command_array);
		exit (127);
	}
}

int	single_parent(pid_t pid, int status)
{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (-1);
}
