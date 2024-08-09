/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/09 14:45:42 by fdessoy-         ###   ########.fr       */
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
				dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				err_msg("'newline'", SYNTAX, 2);		
		}
		else if (!ft_strcmp(array[data->index], "<<"))
		{
			if (array[data->index + 1])
			{
				here_doc(data, array[data->index + 1]);
				dup2(data->fd_in, STDIN_FILENO);
				close(data->fd_in);
				dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				err_msg("'newline'", SYNTAX, 2);		
		}
		data->index++;
	}
}

void	here_doc(t_data *data, char *delimiter)
{
    char    buffer[1024];
    ssize_t bytes_read;

    while (1)
    {
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0)
        {
            perror("read");
            break ;
        }
        buffer[bytes_read] = '\0'; // Null-terminate the string
        if (strncmp(buffer, delimiter, 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0'))
            break ;
        if (write(data->fd_in, buffer, bytes_read) != bytes_read)
            break ;
    }
}