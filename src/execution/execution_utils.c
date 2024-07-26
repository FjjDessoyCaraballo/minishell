/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/26 11:32:13 by fdessoy-         ###   ########.fr       */
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
	{
		printf("Syntax error near unexpected token: ");
		printf("`%s'\n", msg);
	}
	return (err_code);
}