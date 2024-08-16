/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_dups.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:06:30 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/16 13:45:17 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	dup_fds(t_data *data, int child, char **array)
{
	if (find_redirection(array) == SUCCESS)
		data->redirections = true;
	if (data->redirections == true)
		redirections_handling(data, array);
	else
	{
		if (child != 0 && data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		if (child != data->nb_cmds - 1)
			dup2(data->pipe_fd[1], STDOUT_FILENO);
		if (data->piped)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
		}
	}
}

void	open_fdin(t_data *data, char *infile)
{
	errno = 0;
	data->fd_in = open(infile, O_RDONLY);
	if (errno == ENOENT || errno == EACCES || errno == EISDIR)
	{
		// dprintf(2, "entering open_fdin\n");
		if (errno == ENOENT)
			exit_child(infile, NO_FILE);
		else if (errno == EACCES)
			exit_child(infile, FILE_PERMISSION_DENIED);
		else if (errno == EISDIR)
			exit_child(infile, EISDIR);
		close_fds(data);
	}
}

void	open_fdout(t_data *data, char *outfile, int flag)
{
	errno = 0;
	if (!flag)
		data->fd_out = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0664);
	else
		data->fd_out = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0664);
	if (errno == ENOENT || errno == EACCES || errno == EISDIR)
	{
		if (errno == ENOENT)
			exit_child(outfile, NO_FILE);
		else if (errno == EACCES)
			exit_child(outfile, FILE_PERMISSION_DENIED);
		else if (errno == EISDIR)
			exit_child(outfile, EISDIR);
		close_fds(data);
	}
}

void	exit_child(char *file, int err_code)
{
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	if (err_code == NO_FILE)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (err_code == FILE_PERMISSION_DENIED)
		ft_putstr_fd("Permission denied\n", 2);
	else if (err_code == EISDIR)
		ft_putstr_fd("Is a directory\n", 2);
	err_code = 1;
	exit(err_code);
}
