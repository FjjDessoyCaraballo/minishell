/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 20:08:16 by fdessoy-         ###   ########.fr       */
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
		if (tmp->type == COMMAND)
			cmds++;
		tmp = tmp->next;
	}
	tmp = NULL;
	return (cmds);
}

int	err_msg(char *obj, char *msg, int err_code)
{
	if (*obj && msg != NULL)
		printf("%s: ", obj);
	if (*msg && msg != NULL)
		printf("%s\n", msg);
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
}