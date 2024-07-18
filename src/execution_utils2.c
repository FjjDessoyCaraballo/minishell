/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:19:20 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/18 15:02:48 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	dup_fds(t_data *data, int child, t_token *token) // this needs to check if its the last, first or middle child
{
	if (child == 0)
	{
		if (token->prev->type == ARGUMENT)
			open_fdout(data, token->prev->value); // THIS CONSIDERS THAT WE HAVE AN INFILE
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->pipe_fd[1], STDOUT_FILENO);
		close_fds(data);
	}
	else if (child == data->nb_cmds) // there was a -1
	{
		if (token->next->type == REDIRECT || token->next->type == ARGUMENT)
			open_fdout(data, token->next->value); // THIS CONSIDERS THAT WE HAVE OUTFILES
		dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close_fds(data);
	}
	else
	{
		dup2(data->read_end, STDIN_FILENO);
		dup2(data->pipe_fd[1], STDOUT_FILENO);
		close_fds(data);
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

void	close_fds(t_data *data)
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
		ft_putstr_fd("No such file or directory\n", 2);
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