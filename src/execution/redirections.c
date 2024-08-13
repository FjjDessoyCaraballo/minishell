/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/13 10:18:22 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void redirections_handling(t_data *data, char **array)
{
	data->index = 0;
	while (array[data->index])
	{
		if (!ft_strcmp(array[data->index], "<"))
		{
			if (array[data->index + 1])
			{
				open_fdin(data, array[data->index + 1]);
				dup2(data->fd_in, STDIN_FILENO);
				close(data->fd_in);
				dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				err_msg("'newline'", SYNTAX, 2);
		}
		else if (!ft_strcmp(array[data->index], ">"))
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
				err_msg("'newline'", SYNTAX, 2);
		}
		else if (!ft_strcmp(array[data->index], ">>"))
		{
			if (array[data->index + 1])
			{
				open_fdout(data, array[data->index + 1], 0);
				dup2(data->fd_in, STDIN_FILENO);
				close(data->fd_in);
				if (data->piped == true)
					dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				err_msg("'newline'", SYNTAX, 2);		
		}
		else if (!ft_strcmp(array[data->index], "+"))
		{
			if (array[data->index + 1])
			{
				handle_heredoc(data, array[data->index + 1]);
				// here_doc(data, array[data->index + 1]);
				// dup2(data->fd_in, STDIN_FILENO);
				// close(data->fd_in);
				// dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				err_msg("'newline'", SYNTAX, 2);		
		}
		data->index++;
	}
}

void	handle_heredoc(t_data *data, char *delimiter)
{
	char	*buffer;
	
	buffer = here_doc(delimiter);
	write(data->pipe_fd[1], buffer, ft_strlen(buffer));
	free(buffer);
	close(data->pipe_fd[1]);
	dup2(data->pipe_fd[0], STDIN_FILENO);
	close(data->pipe_fd[0]);
}

char	*here_doc(char *delimiter)
{
	char	*input;
	char	*buffer;
	char	*tmp1;
	char	*tmp2;

	buffer = ft_strdup("");
	malloc_check_message(buffer);
	while (1)
	{
		input = readline("8==D ");
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		tmp1 = buffer;
		buffer = ft_strjoin(buffer, input);
		malloc_check_message(buffer);
		tmp2 = buffer;
		buffer = ft_strjoin(buffer, "\n");
		malloc_check_message(buffer);
		free_null(tmp2);
		free_null(tmp1);
		free_null(input);
	}
	free_null(input);
	return (buffer);
}

int	find_redirection(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (!ft_strcmp(array[i], ">>")
			|| !ft_strcmp(array[i], "<<")
			|| !ft_strcmp(array[i], ">")
			|| !ft_strcmp(array[i], "<"))
			return (SUCCESS);
		i++;
	}
	return (FAILURE);
}