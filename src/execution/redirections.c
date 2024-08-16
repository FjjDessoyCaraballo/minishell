/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/16 13:11:34 by fdessoy-         ###   ########.fr       */
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
				if (data->piped)
					dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				exit(err_msg("'newline'", SYNTAX, 2));
		}
		if (!ft_strcmp(array[data->index], ">"))
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
		else if (!ft_strncmp(array[data->index], ">>", 2))
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
						exit(err_msg(NULL, HEREDOC_FAILURE, 1));
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

void here_doc(t_data *data, char *delimiter)
{
	char	*input;

	if (data->piped == false)
	{
		data->fd_in = open("/tmp/heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0664);
		if (data->fd_in < 0)
			exit(err_msg(NULL, HEREDOC_FAILURE, 1));
	}
	while (1)
	{
		input = readline("8==D ");
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		if (data->piped)
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
	if (data->piped == false)
		close(data->fd_in);
}

// void here_doc(t_data *data, char *delimiter)
// {
//     char    *input;
//     int     pipe_fd[2];  // Pipe file descriptors

//     // Create a pipe
//     if (pipe(pipe_fd) == -1)
//         exit(err_msg(NULL, "Failed to create pipe", 1));

//     while (1)
//     {
//         input = readline("8==D ");
//         if (!input || !ft_strncmp(input, delimiter, ft_strlen(delimiter)))
//             break;

//         // Write input to the write end of the pipe
//         write(pipe_fd[1], input, ft_strlen(input));
//         write(pipe_fd[1], "\n", 1);
//         free(input);
//     }
//     if (input)
//         free(input);

//     // Close the write end of the pipe after writing all input
//     close(pipe_fd[1]);

//     // If the command is piped, continue as before
//     if (data->piped)
//     {
//         // Redirect stdin to the read end of the pipe
//         dup2(pipe_fd[0], STDIN_FILENO);
//     }
//     else
//     {
//         // For non-piped case, redirect stdin to the read end of the pipe
//         data->fd_in = pipe_fd[0];
//         dup2(data->fd_in, STDIN_FILENO);
//     }
//     // Close the read end of the pipe after redirecting
//     close(pipe_fd[0]);
// }



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