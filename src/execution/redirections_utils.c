/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:28:13 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/20 14:43:21 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	input_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdin(data, array[data->index + 1]);
		dup2(data->fd_in, STDIN_FILENO);
		close(data->fd_in);
		if (data->piped == true)
			dup2(data->pipe_fd[1], STDOUT_FILENO);	
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	output_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 1);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	append_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 0);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));		
}
	
void	heredoc_redirection(t_data *data, char **array)
{
	here_doc(data, array[data->index + 1]);
	dprintf(2,"data->pipe_fd[1]: %i\n", data->pipe_fd[1]);
	dprintf(2, "data->fd_in: %i\n", data->fd_in);
	if (array[data->index + 1])
	{
		if (data->piped == true)
		{
			dup2(data->pipe_fd[0], STDIN_FILENO);
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
		}
		else
		{
			data->fd_in = open("/tmp/heredoc_tmp", O_RDONLY);
			if (data->fd_in < 0)
				exit(err_msg(NULL, "error", 1));
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
		}
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));	
}