/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 12:46:16 by fdessoy-         ###   ########.fr       */
=======
/*   Created: 2024/07/15 17:33:52 by walnaimi          #+#    #+#             */
/*   Updated: 2024/07/19 17:38:57 by walnaimi         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_check(char *token, t_token *current_token, char **builtins)
{
	int i;
	i = 0;
	while(builtins[i] != NULL)
	{
		if(ft_strcmp(token, builtins[i]) == SUCCESS)
		{
			current_token->value = ft_strdup(token);
			current_token->type = BUILTIN;
			return (SUCCESS);
		}
		i++;
	}

		return (FAILURE);
}

void print_binary_paths(t_data *data)
{
    for (int i = 0; data->binary_paths[i] != NULL; i++)
    {
        printf("Path %d: %s\n", i, data->binary_paths[i]);
    }

	if (!data->binary_paths)
    {
        printf("Binary paths are NULL\n");
        return;
    }
}

int ft_command_check(char *token, t_token *current_token, t_data *data)
{
    if (current_token->type != UNKNOWN)
        return FAILURE;
<<<<<<< HEAD
    char **paths = ft_split(bin_paths, ':');
=======

    char **paths = ft_split(data->bin, ':');
	//char **paths = data->binary_paths;
	//print_binary_paths(data);
>>>>>>> main
    char *executable_path = loop_path_for_binary(token, paths);
    if (executable_path != NULL)
    {
        // Find the last '/' character to separate the path and name
        char *last_slash = strrchr(executable_path, '/');
        if (last_slash)
        {
            int path_len = last_slash - executable_path + 1;
            current_token->path = ft_strndup(executable_path, path_len);
            current_token->value = ft_strdup(last_slash + 1);
        }
        else
        {
            current_token->path = NULL;
            current_token->value = strdup(executable_path);
        }

        current_token->type = COMMAND;
		free_my_boi(paths);
        free(executable_path);  // Free the allocated path
        return SUCCESS;
    }
	free_my_boi(paths);
    return FAILURE;
}


int ft_pipe_check(char *token, t_token *current_token)
{
		if (ft_strcmp(token,"|") == 0)
		{
			current_token->value = token;
			current_token->type = PIPE;
			return(SUCCESS);
		}
	return(FAILURE);
}

int	ft_redirect_op_check(char *token,t_token *current_token, char **redirect)
{
	int i;
	i = 0;
	while(redirect[i] != NULL)
	{
		if(ft_strcmp(token,redirect[i]) == SUCCESS)
		{
			current_token->value = ft_strdup(token);
			current_token->type = REDIRECT;
			return(SUCCESS);
		}
		i++;
	}
	return(FAILURE);
}

int	ft_argument_check(char *token, t_token *current_token)
{
	current_token->value = token;
	current_token->type = ARGUMENT;
	return SUCCESS;
}
