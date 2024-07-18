/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/18 14:03:44 by fdessoy-         ###   ########.fr       */
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
	t_token	*tmp;
	
	tmp = token;
	i = 0;
	while (tmp != NULL)
	{
		while (tmp->type != COMMAND)
			tmp = tmp->next;
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
		tmp = tmp->next;
		i++;
	}
	return (FAILURE);
}

/* at this point, our token should always be the command */
void	plumber_kindergarten(t_data *data, t_token *token, int child)
{
	dup_fds(data, child, token);
	execve(token->path, token->value, data->env);	
	return ;
}

void	dup_fds(t_data *data, int child, t_token *token) // this needs to check if its the last, first or middle child
{
	if (child == 0)
	{
		if (token->prev->type == ARGUMENT)
			open_infile(data, token->prev->value); // THIS CONSIDERS THAT WE HAVE AN INFILE
		dup2_or_exit(data, data->fd_in, STDIN_FILENO);
		dup2_or_exit(data, data->pipe_fd[1], STDOUT_FILENO);
		close_open_fds(data);
	}
	else if (child == data->nb_cmds) // there was a -1
	{
		if (token->next->type == REDIRECT || token->next->type == ARGUMENT)
			open_outfile(data, token->next->value); // THIS CONSIDERS THAT WE HAVE OUTFILES
		dup2_or_exit(data, data->read_end, STDIN_FILENO);
		dup2_or_exit(data, data->fd_out, STDOUT_FILENO);
		close_open_fds(data);
	}
	else
	{
		dup2_or_exit(data, data->read_end, STDIN_FILENO);
		dup2_or_exit(data, data->pipe_fd[1], STDOUT_FILENO);
		close_open_fds(data);
	}
}

void	open_fdin(t_data *data, char *infile)
{
	data->fd_in = open(infile, O_RDONLY);
	if (errno == ENOENT)
		exit_child(infile, NO_FILE);
	else if (errno == EACCES)
		exit_child(infile, FILE_PERMISSION_DENIED);
	else if (errno == EISDIR)
		exit_child(infile, EISDIR);
}

void	open_fdout(t_data *data, char *outfile)
{
	data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (errno == ENOENT)
		exit_child(outfile, NO_FILE);
	else if (errno == EACCES)
		exit_child(outfile, FILE_PERMISSION_DENIED);
	else if (errno == EISDIR)
		exit_child(outfile, EISDIR);
}

void	close_open_fds(t_data *data)
{
	if (data->pipe_fd[0] != -1)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] != -1)
		close(data->pipe_fd[1]);
	if (data->fd_in != -1)
		close(data->fd_in);
	if (data->fd_in != -1)
		close (data->fd_out);
	if (data->read_end != -1)
		close(data->read_end);
}

void	exit_child(char *file, int err_code)
{
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	if (err_code == NO_FILE)
		ft_putstr_fd("No such file or directory\n");
	else if (err_code == FILE_PERMISSION_DENIED)
	{
		ft_putstr_fd("Permission denied\n", 2);
		err_code = 1;
	}
	else if (err_code == EISDIR)
	{
		ft_putstr_fd("Is a directory\n", 2);
		err_code = 1;
	}
	exit(err_code);
}

/* execve() second argument has to be an array of the command and its flags */
// int lonely_execution(t_data *data, t_token *token, t_env **env_ll)
// {
// 	char	*path;
	
// 	env_arr_updater(data, env_ll);
// 	path = access_path(data->binary_paths, token->value);
// 	data->cmd = ft_split(token->value, ' ');
// 	if (!path)
// 		return (127);
// 	if (execve(path, data->cmd, data->env) == -1)
// 	{
// 		printf("we got here\n");
// 		ft_putstr_fd("Command not found: \n", 2);
// 		ft_putendl_fd(token->value, 2);
// 		return (127);
// 	}
// 	return (SUCCESS);
// }


