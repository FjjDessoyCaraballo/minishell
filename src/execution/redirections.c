/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 12:26:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	last_heredoc(char **array, t_data *data)
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
	if (!last_index)
		data->heredoc_exist = true;
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

void	redirections_handling(t_data *data, char **array, t_env **env_ll)
{
	data->last_heredoc_index = last_heredoc(array, data);
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
			if (data->index == data->last_heredoc_index)
			{
				heredoc_redirection(data, array, env_ll);
				write(data->sync_pipe[1], "1", 1);
			}
		}
		data->index++;
	}
}

int	here_doc(char *delimiter, t_data *data, t_env **env_ll)
{
	static char	*input;
	int			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		exit(err_msg(NULL, "pipe error", 1));
	while (1)
	{
		g_exit_code = HEREDOC;
		input = readline("8==D ");
		if (!input)
		{
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		input = expand_env_variables(input, data, env_ll);
		write(pipe_fd[1], input, ft_strlen(input));
		write(pipe_fd[1], "\n", 1);
		free(input);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
