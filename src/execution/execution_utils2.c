/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:19:20 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/23 17:08:02 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void dup_fds(t_data *data, int child, int fd_flag, char *file)
{
    if (child == 0) // first child
	{
		if (fd_flag == 1)
		{
			open_fdin(data, file);
			dup2(data->fd, STDIN_FILENO);
			close (data->fd);
		}
        dup2(data->pipe_fd[1], STDOUT_FILENO);
	}
	else if (child == data->nb_cmds) // last child
    {
		if (fd_flag == 1)
		{
			open_fdout(data, file);
			dup2(data->fd_out, STDIN_FILENO);
			close(data->fd_out);
		}
	    dup2(data->read_end, STDIN_FILENO);
	}
	else 
	{
		if (file)
		{
			open_fdin(data, file);
			dup2(data->fd_in, STDIN_FILENO);
			close (data->fd_in);
		}
		else
        	dup2(data->read_end, STDIN_FILENO);
        dup2(data->pipe_fd[1], STDOUT_FILENO);
    }
    close(data->pipe_fd[0]);
    close(data->pipe_fd[1]);
}

void	open_fdin(t_data *data, char *infile)
{
	errno = 0;	
	data->fd_in = open(infile, O_RDONLY);
	if (errno == ENOENT)
	{
		close_fds(data);
		exit_child(infile, NO_FILE);
	}
	else if (errno == EACCES)
	{
		close_fds(data);
		exit_child(infile, FILE_PERMISSION_DENIED);
	}
	else if (errno == EISDIR)
	{
		close_fds(data);
		exit_child(infile, EISDIR);
	}
}

void	open_fdout(t_data *data, char *outfile)
{
	errno = 0;	
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