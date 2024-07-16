/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/16 15:35:01 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;

Execution should happen within child process, otherwise it quits the whole thang.
Therefore, iteration might be neccessary for either single execution or builtin
*/
int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;

	token = data->token;
	token->value = data->line_read;
	
	while (token != NULL)
	{ 
		if (token->type != PIPE || token->type != REDIRECT || token->next == NULL)
			built_ins(data, token, env_ll);
		else
		{
			data->fd = how_many_children(data, token) + 1;
			if (!data->fd)
				return (FAILURE);	
			data->status = crack_pipe(data, token);
		
		}
		token = token->next;
	}
	return (SUCCESS);
}

int	crack_pipe(t_data *data, t_token *token) // environment pointer will probably come back here
{
	int		i;
	pid_t	pids[data->processes];

	if (pipe(data->fd) == -1)
	{
		perror("Plumbing error: ");
		// gotta close FDs
		return (141);
	}
	i = 0;
	while (i < data->processes)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			if (i == (data->processes - 1))
				waitpid(pids[0], &data->status, 0);
			else
				waitpid(pids[i + 1], &data->status, 0);
			close_all_fds(data->fd);
			return (127);
		}
		i++;
	}
	//DUMMY USE OF TOKEN
	if (token->next->next->next->next->type == UNKNOWN)
		close_all_fds(data->fd);
	return (FAILURE);
}


/* execve() second argument has to be an array of the command and its flags */
int lonely_execution(t_data *data, t_token *token, t_env **env_ll)
{
	char	*path;
	
	env_arr_updater(data, env_ll);
	path = access_path(data->binary_paths, token->value);
	data->cmd = ft_split(token->value, ' ');
	if (!path)
		return (127);
	if (execve(path, data->cmd, data->env) == -1)
	{
		printf("we got here\n");
		ft_putstr_fd("Command not found: \n", 2);
		ft_putendl_fd(token->value, 2);
		return (127);
	}
	return (SUCCESS);
}


