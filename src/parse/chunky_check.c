/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 10:03:48 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_check(char *token, t_token *current_token, char **builtins)
{
	int	i;

	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strcmp(token, builtins[i]) == SUCCESS)
		{
			current_token->value = ft_strdup(token);
			current_token->type = BUILTIN;
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

void	print_binary_paths(t_data *data)
{
	for (int i = 0; data->binary_paths[i] != NULL; i++) // NEED TO REMOVE!!
	{
		printf("Path %d: %s\n", i, data->binary_paths[i]);
	}
	if (!data->binary_paths)
	{
		printf("Binary paths are NULL\n");
		return ;
	}
}

int	ft_command_check(char *token, t_token *current_token, t_data *data)
{
	char	**paths;
	char	*executable_path;
	char	*last_slash;
	int 	path_len; 

	if (current_token->type != UNKNOWN)
		return (FAILURE);
	paths = ft_split(data->bin, ':');
	//char **paths = data->binary_paths;
	//print_binary_paths(data);
	executable_path = loop_path_for_binary(token, paths);
	if (executable_path != NULL)
	{
		// Find the last '/' character to separate the path and name
		last_slash = ft_strrchr(executable_path, '/');
		if (last_slash)
		{
			path_len = last_slash - executable_path + 1;
			current_token->path = ft_strndup(executable_path, path_len);
			current_token->value = ft_strdup(last_slash + 1);
		}
		else
		{
			current_token->path = NULL;
			current_token->value = ft_strdup(executable_path);
		}
		current_token->type = COMMAND;
		free_my_boi(paths);
		free(executable_path);  // Free the allocated path
		return (SUCCESS);
	}
	free_my_boi(paths);
	return (FAILURE);
}

int	ft_pipe_check(char *token, t_token *current_token)
{
		if (ft_strcmp(token,"|") == 0)
		{
			current_token->value = token;
			current_token->type = PIPE;
			return (SUCCESS);
		}
	return (FAILURE);
}

int	redirect_op_check(char *token,t_token *current_token, char **redirect)
{
	int	i;

	i = 0;
	while (redirect[i] != NULL)
	{
		if (ft_strcmp(token,redirect[i]) == SUCCESS)
		{
			current_token->value = ft_strdup(token);
			if (i == 0)
				current_token->type = RED_OUT;// > output
			else if (i == 1)
				current_token->type = APPEND;//  >>output
			else if (i == 2)
				current_token->type = RED_IN;//  < input
			else
				current_token->type = HEREDOC;// <<input
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}
