/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/17 13:24:34 by fdessoy-         ###   ########.fr       */
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

int	*how_many_children(t_data *data, t_token *token)
{
	int		*fd;
	int		processes;
	t_token	*tmp;
	
	processes = 0;
	tmp = token;
	while (tmp != NULL)
	{
		if (tmp->type == PIPE)
			processes++;
		tmp = tmp->next;
	}
	fd = (int *)malloc(sizeof(int) * (processes + 1));
	if (!fd)
		return (NULL);
	data->processes = processes + 1;
	tmp = NULL;
	return (fd);
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