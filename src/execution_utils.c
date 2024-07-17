/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/17 16:14:09 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	how_many_children(t_data *data, t_token *token)
{
	int		cmds;
	int		*fd;
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
	data->fd = (int *)malloc(sizeof(int) * (cmds * 2));
	if (!data->fd)
		return ;
	data->nb_cmds = cmds;
}


void	close_all_fds(int *fd)
{
	int	i;

	i = 0;
	while (fd[i])
	{
		close(fd[i]);
		i++;
	}
}

int	err_pipes(char *msg, int err_code)
{
	if (*msg)
		ft_printf("%s: command not found\n", msg);
	return (err_code);
}

	// fds = (cmds * 2);
	// data->fd = (int *)fds;