/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 15:56:13 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;

Execution should probably depend on what we find in the line. 
Therefore, iteration might be neccessary for either single execution or builtin
*/
int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;

	token = data->token;
	token->value = data->line_read;
	if (token->type == BUILTIN)
		built_ins(data, token, env_ll);
	else if (token->type == COMMAND)
		data->exit_status = lonely_execution(data, token, env_ll);
	return (SUCCESS);
}

int lonely_execution(t_data *data, t_token *token, t_env **env_ll)
{
	int		i;
	char	*path;

	i = 0;
	env_arr_updater(data, (*env_ll));
	printf("token->value: %s\n", token->value);
	while (data->binary_paths[i])
	{
		path = access_path(data->binary_paths, token->value);
		if (!path)
			return (127);
		if (execve(path, &token->value, data->env) == -1)
		{
			ft_putstr_fd("Command not found: \n", 2);
			ft_putendl_fd(token->value, 2);
			return (127);
		}
	}
	return (SUCCESS);
}

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
				return (ft_strrchr(curr_path, '/'));
			}
			ft_putstr_fd("Command not found: ", 2);
			ft_putendl_fd(cmd, 2);
		}
		free(curr_path);
		i++;
	}
	return (NULL);
}



