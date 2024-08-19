/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 15:42:22 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void redirections_handling(t_data *data, char **array)
{
	data->index = 0;
	while (array[data->index])
	{
		if (!ft_strcmp(array[data->index], "<"))
			input_redirection(data, array);
		else if (!ft_strcmp(array[data->index], ">"))
			output_redirection(data, array);
		else if (!ft_strcmp(array[data->index], ">>"))
			append_redirection(data, array);	
		else if (!ft_strcmp(array[data->index], "<<"))
			heredoc_redirection(data, array);
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
	if (data->piped == false)
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