/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/17 16:27:53 by fdessoy-         ###   ########.fr       */
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

		// else if ((token->type == COMMAND) || (token->type == BUILTIN
		// 	&& token->next->type == PIPE) || (token->type == BUILTIN
		// 	&& token->next->type == FLAG && token->next->next->type == PIPE))

	// if (token->next == NULL)
	// 	return (err_pipes(NULL, 148));

// if (token->type != PIPE || token->type != REDIRECT || token->next == NULL
// 			|| (token->type == BUILTIN && token->next == NULL) || token->type != COMMAND)

			
int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;

	token = data->token;
	token->value = data->line_read;
	how_many_children(data, token);
	if (token->type == COMMAND || (token->type == ARGUMENT
		&& token->next->type == REDIRECT) || token->type == REDIRECT)
	{
		data->status = crack_pipe(data, token);
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
		// printf("token->value: %s\ntoken->type: %i\n", token->value, token->type);

int	crack_pipe(t_data *data, t_token *token) // we're getting inside children
{
	int		i;
	pid_t	pids[data->nb_cmds];

	i = 0;
	while (i < data->nb_cmds)
	{
		if (pipe(data->pipe_fd) == -1)
			return (err_pipes("Broken pipe\n", 141));
		pids[i] = fork();
		if (pids[i] < 0)
		{
			close(data->fd[i]);
			return (err_pipes("Failed to fork\n", -1));
		}
		if (pids[i] == 0)
			plumber_kindergarten(data, token, i);
		data->read_end = data->pipe_fd[0];
		close(data->pipe_fd[1]);
		i++;
	}
	return (FAILURE);
}

void	plumber_kindergarten(t_data *data, t_token *token, int child)
{
	t_token	*tmp;

	tmp = token;
	while (token != NULL)
	{
		if (token->type == COMMAND) // this needs to check if its the last, first or middle child
			dup_fds(data, child, token)
		tmp = tmp->next;
	}
	tmp = NULL;
	return ;
}

// PROTOTYPE!!!
void	dup_fds(t_data *data, int child, t_token *token)// this needs to check if its the last, first or middle child
{
	if (child == 0)
	{
		open_infile(data);
		dup2_or_exit(data, data->in_fd, STDIN_FILENO);
		dup2_or_exit(data, data->pipe_fd[1], STDOUT_FILENO);
		close_open_fds(data);
	}
	else if (child == data->nb_cmds - 1)
	{
		open_outfile(data);
		dup2_or_exit(data, data->read_end, STDIN_FILENO);
		dup2_or_exit(data, data->out_fd, STDOUT_FILENO);
		close_open_fds(data);
	}
	else
	{
		dup2_or_exit(data, data->read_end, STDIN_FILENO);
		dup2_or_exit(data, data->pipe_fd[1], STDOUT_FILENO);
		close_open_fds(data);
	}
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


