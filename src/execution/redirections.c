/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/20 19:16:56 by fdessoy-         ###   ########.fr       */
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