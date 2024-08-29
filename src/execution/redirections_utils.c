/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:08:39 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:08:41 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_helper(t_data *data, char **array, int heredoc, t_env **env)
{
	if (!ft_strncmp(array[data->index], "<", 1)
		&& ft_strlen(array[data->index]) == 1)
		input_redirection(data, array);
	else if (!ft_strncmp(array[data->index], ">", 1)
		&& ft_strlen(array[data->index]) == 1)
		output_redirection(data, array);
	else if (!ft_strncmp(array[data->index], ">>", 2)
		&& ft_strlen(array[data->index]) == 2)
		append_redirection(data, array);
	else if (!ft_strncmp(array[data->index], "<<", 2)
		&& ft_strlen(array[data->index]) == 2)
	{
		if (data->index == heredoc)
		{
			heredoc_redirection(data, array, env);
			write(data->sync_pipe[1], "1", 1);
		}
	}
}

void	input_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		if (!access(array[data->index + 1], F_OK))
		{
			open_fdin(data, array[data->index + 1]);
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
			if (data->piped == true)
				dup2(data->pipe_fd[1], STDOUT_FILENO);
		}
		else
			exit(err_msg(array[data->index + 1], FILE_ERROR, 2));
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

void	heredoc_redirection(t_data *data, char **array, t_env **env_ll)
{
	if (array[data->index + 1])
	{
		data->fd_in = here_doc(array[data->index + 1], data, env_ll);
		dup2(data->fd_in, STDIN_FILENO);
		close(data->fd_in);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}
