/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/21 13:06:41 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	last_heredoc(char **array)
{
	int	i;
	int	last_index;

	last_index = -1;
	i = 0;
	while (array[i])
	{
		if (!ft_strncmp(array[i], "<<", 2)
			&& ft_strlen(array[i]) == 2)
			last_index = i;
		i++;
	}
	dprintf(2, "last_heredoc_index: %i\n", last_index);
	return (last_index);
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

void redirections_handling(t_data *data, char **array)
{
	int	last_heredoc_index;

	last_heredoc_index = last_heredoc(array);
	data->index = 0;
	while (array[data->index])
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
			if (data->index == last_heredoc_index)
			{
				heredoc_redirection(data, array);
				write(data->sync_pipe[1], "1", 1);
			}
		}
		data->index++;
	}
}

int here_doc(char *delimiter)
{
    static char *input;

    int pipe_fd[2];
    
    if(pipe(pipe_fd) == -1)
        exit(err_msg(NULL, "pipe  error", 1));
    while (1)
    {
        input = readline("8==D ");
        if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
            break ;
        write(pipe_fd[1], input, ft_strlen(input));
        write(pipe_fd[1], "\n", 1);
        free(input);
    }
    close(pipe_fd[1]);
    return(pipe_fd[0]);
}
