/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 12:00:08 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*access_path(char **path, char *cmd)
{
	int		i;
	char	*curr_path;

	i = 0;

	while (path[i])
	{
		curr_path = ft_strsjoin(path[i], cmd, '/');
		if (!access(curr_path, F_OK))
		{
			if (!access(curr_path, X_OK))
			{
				free_array(path);
				return (curr_path);
			}
			ft_putstr_fd("Command not found: ", 2);
			ft_putendl_fd(cmd, 2);
		}
		free(curr_path);
		i++;
	}
	return (NULL);
}

int	how_many_children(t_token *token)
{
	int		cmds;
	t_token	*tmp;
	
	cmds = 0;
	tmp = token;
	while (tmp != NULL)
	{
		if (tmp->type == PIPE)
			cmds++;
		tmp = tmp->next;
	}
	
	tmp = NULL;
	return (cmds + 1);
}

int	err_pipes(char *msg, int err_code)
{
	if (*msg)
	{
		printf("Syntax error near unexpected token: ");
		printf("`%s'\n", msg);
	}
	return (err_code);
}

void close_fds(t_data *data)
{
    if (data->pipe_fd[0] != 0)
        close(data->pipe_fd[0]);
    if (data->pipe_fd[1] != 0)
        close(data->pipe_fd[1]);
    if (data->fd_in != 0)
        close(data->fd_in);
    if (data->fd_out != 0)
        close(data->fd_out);
    if (data->read_end != 0)
        close(data->read_end);

	// printf("data->pipe_fd[0] %i\n", data->pipe_fd[0]);
	// printf("data->pipe_fd[1] %i\n", data->pipe_fd[1]);
	// printf("data->read_end %i\n", data->read_end);
	// printf("data->fd_in %i\n", data->fd_in);
	// printf("data->fd_out %i\n", data->fd_out);
}

    // if (data->pipe_fd[0] != -1)
	// {
    //     status = close(data->pipe_fd[0]);
	// 	// printf("status of data->pipe_fd[0] %i\n", status);
    //     data->pipe_fd[0] = 0; // Resetting to -1 after closing
    // }
    // if (data->pipe_fd[1] != -1)
	// {
	// 	status = 0;
    //     status = close(data->pipe_fd[1]);
	// 	printf("status of data->pipe_fd[1] %i\n", status);
    //     data->pipe_fd[1] = 0; // Resetting to -1 after closing
    // }
    // if (data->fd_in != -1) // && data->fd_in != STDIN_FILENO
	// {
    //     close(data->fd_in);
    // }
    // if (data->fd_out != -1 && data->fd_out != STDOUT_FILENO) 
	// {
	// 	status = 0;
    //     status = close(data->fd_out);
	// 	printf("status of data->fd_out %i\n", status);
    //     data->fd_out = 0; // Resetting to -1 after closing
    // }
    // if (data->read_end != -1 && data->read_end != STDIN_FILENO) 
	// {
	// 	status = 0;
    //     status = close(data->read_end);
	// 	printf("status of data->read %i\n", status);
    //     data->read_end = 0; // Resetting to -1 after closing
	// }