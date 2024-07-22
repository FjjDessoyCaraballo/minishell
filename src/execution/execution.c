/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 12:49:30 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;

Execution should happen within child process, otherwise it quits the whole thang.
Therefore, iteration might be neccessary for either single execution or builtin
*/	
static int	token_printer(t_data *data)
{
	int i = 0;
	
	while (data->cmd_a[i])
	{
		printf("[%s]\n", data->cmd_a[i]);
		i++;
	}
	// head = NULL;
	return (SUCCESS);
}

int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;
	
	token = data->token;
	token_printer(data);
	if (how_many_children(data, token) == 1 && !search_token_type(token, PIPE))
	{
		// token_printer(token);
		single_execution(data, token, env_ll);
	}
	if (how_many_children(data, token) > 3 && search_token_type(token, PIPE))
	{
		token = find_token(token, COMMAND);
		data->status = multiple_cmds(data, token, env_ll);
	}
	data->status = built_in_or_garbage(data, env_ll, token);
	if (data->status != 0)
		return (data->status);
	return (148);
}



int	built_in_or_garbage(t_data *data, t_env **env_ll, t_token *token)
{
	t_token *tmp;
	
	tmp = token;
	while (tmp != NULL)
	{
		if (token->type == BUILTIN)
			return (built_ins(data, token, env_ll));
		else if (token->type == ARGUMENT)
			return (err_pipes(token->value, 127));
		tmp = tmp->next;
	}
	return (0);
}

int	multiple_cmds(t_data *data, t_token *token, t_env **env_ll) // we're getting inside children
{
	int		i;
	pid_t	pids;
	t_token	*tmp;
	
	tmp = token;
	i = 0;
	while (tmp != NULL)
	{		
		if (pipe(data->pipe_fd) == -1)
			return (err_pipes("Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close(data->fd[i]);
			return (err_pipes("Failed to fork\n", -1));
		}
		if (pids == 0)
			piped_execution(data, token, env_ll, i);
		data->read_end = data->pipe_fd[0];
		if (data->nb_cmds > 1)
			close(data->pipe_fd[1]);
		tmp = tmp->next;
		i++;
	}
	return (FAILURE);
}

/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
void	piped_execution(t_data *data, t_token *token, t_env **env_ll, int child)
{
	char	*path;
	char	**env;
	
	dup_fds(data, child, token);
	env = env_arr_updater(env_ll);
	path = ft_strsjoin(token->path, token->value, '/');
	if (execve(path, tokens_to_array(token), env) == -1)	
	{
		free_array(env);
		free_data(data, path, env_ll);
		exit (data->status = err_pipes(token->value, 127));
	}
}

/**
 * This function takes care of executing whole commands with no piping.
 */
void	single_execution(t_data *data, t_token *token, t_env **env_ll)
{
	pid_t	pid;
	char	*path;
	char	**env;

	pid = fork();
	if (pid < 0)
	{
		free_ll((*env_ll));
		free_data(data, NULL, env_ll);
		perror("fork");
		data->status = -1;
	}
	else if (pid == 0)
	{
		path = ft_strsjoin(token->path, token->value, '/');
		env = env_arr_updater(env_ll);
		if (execve(path, data->cmd_a, env) == -1)
		{
			free_array(env);
			free_data(data, path, env_ll);
			exit (data->status = err_pipes(token->value, 127));
		} 
	}
	return ;
}

void	free_data(t_data *data, char *path, t_env **env)
{
	int	i;

	i = 0;
	while (data->binary_paths[i])
		free(data->binary_paths[i++]);
	free(data->binary_paths);
	free_ll(*env);
	if (path)
		free(path);
	free(data);
}