/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:09:12 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/15 15:12:30 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * This is the second part of the execution where we are going to
 * check if we have the redirection flag (int redirect) and we are
 * parsing the commands differently if we do.
 * 
 * Redirections here take an even more strict definition:
 * - "%> cat << EOF | cat > outfile"
 * 
 * Therefore, in here, redirections will be strictly "<" and ">", while
 * HERE_DOC and APPEND will have explicit naming because they are able
 * to take arguments beforehand.
 * 
 * [placeholder for more documentation]
 */
void	ft_exec(t_data *data, char **cmd_array)
{
	static char	*path;

	if (ft_strchr(cmd_array[0], '/') == NULL)
	{
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
		if (!path)
		{
			err_msg(cmd_array[0], NO_EXEC, 0);
			free_data(data, NULL, cmd_array);
			exit(127);
		}
	}
	if (!path)
	{
		if (execve(cmd_array[0], cmd_array, data->env) == -1)	
		{
			err_msg(cmd_array[0], NO_EXEC, 0);
			free_data(data, NULL, cmd_array);
			exit(127);
		}
	}
	if (execve(path, cmd_array, data->env) == -1)
	{
		err_msg(cmd_array[0], NO_EXEC, 0);
		free_data(data, path, cmd_array);
		exit(127);
	}
}

/**
 * we do not know where in the token list we execute, but we have
 * the array to compare to. As we are inside the child, the array
 * element 0 here will tell us which built-in we will execute. 
 * Therefore, all we need to do afterwards is recall the built-ins
 */
t_token	*ft_builtin_exec(char **array, t_token *token)
{
	int		i;
	t_token	*builtin_token;

	i = 0;
	while (array[i])
	{
		builtin_token = token;
		while (builtin_token)
		{
			if (!ft_strcmp(array[i], builtin_token->value))
			{
				if (builtin_token->type == BUILTIN)
					return (builtin_token);
			}
			builtin_token = builtin_token->next;
		}
		i++;
	}
	builtin_token = NULL;
	return (NULL);
}
