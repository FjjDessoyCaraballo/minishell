/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 16:40:20 by fdessoy-         ###   ########.fr       */
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
				exit(err_msg("'newline'", SYNTAX, 2));
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
				exit(err_msg("'newline'", SYNTAX, 2));
		}
		else if (!ft_strcmp(array[data->index], ">>"))
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
		else if (!ft_strcmp(array[data->index], "<<"))
		{
			if (array[data->index + 1])
			{
				here_doc(data, array[data->index + 1]);
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
						exit(err_msg(NULL, "generic error", 1));
					dup2(data->fd_in, STDIN_FILENO);
					close(data->fd_in);
				}
			}
			else
				exit(err_msg("'newline'", SYNTAX, 2));
		}
		data->index++;
	}
}

void	here_doc(t_data *data, char *delimiter)
{
	char	*input;

	data->fd_in = open("/tmp/heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (data->fd_in < 0)
		exit(err_msg(NULL, "generic error", 1));
	while (1)
	{
		input = get_next_line(0);
		// input = readline("XXX ");
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		if (data->piped == true)
		{
			write(data->pipe_fd[1], input, ft_strlen(input));
			write(data->pipe_fd[1], "\n", 1);
		}
		else
		{			
			write(data->fd_in, input, ft_strlen(input));
			write(data->fd_in, "\n", ft_strlen(input));
		}
		free(input);
	}
	close(data->fd_in);
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