/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/20 03:10:26 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_check(char *token, t_token *current_token)
{
	if(ft_strncmp(token, "echo", 5) == 0
	|| ft_strncmp(token, "exit", 5) == 0
	|| ft_strncmp(token, "pwd", 4) == 0
	|| ft_strncmp(token, "cd", 3) == 0
	|| ft_strncmp(token, "export", 7) == 0
	|| ft_strncmp(token, "unset", 6) == 0
	|| ft_strncmp(token, "env", 3) == 0)
		{
			current_token->value = ft_strdup(token);
			current_token->type = BUILTIN;
			return (0);
		}
		return (1);
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
    char **paths = ft_split(data->bin, ':');
    char *executable_path = loop_path_for_binary(token, paths);
    if (executable_path != NULL)
    {
        char *last_slash = ft_strrchr(executable_path, '/');// Find the last '/' character to separate the path and name
        if (last_slash)
        {
            int path_len = last_slash - executable_path + 1;
            current_token->path = ft_strndup(executable_path, path_len); // 2 -> check_command 28
            current_token->value = ft_strdup(last_slash + 1);
        }
        else
        {
            current_token->path = NULL;
            current_token->value = ft_strdup(executable_path);
        }
        current_token->type = COMMAND;
		//free_my_boi(paths);
        //free(executable_path); <- the way I free this shi causes some problems in one edge case (thank armin for help :3)
        return 0;
    }
	free_my_boi(paths);
    return 1;
}

int ft_pipe_check(char *token, t_token *current_token)
{
		if (ft_strcmp(token,"|") == 0)
		{
			current_token->value = token;
			current_token->type = PIPE;
			return(0);
		}
	return(1);
}

int	ft_redirect_op_check(char *token,t_token *current_token)
{
	if(ft_strncmp(token,">",2) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_OUT;// > output
		return(0);
	}
	else if (ft_strncmp(token,">>",3) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = APPEND;// >> output
		return(0);
	}
	else if (ft_strncmp(token,"<",2) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_IN;// < input
		return(0);
	}
	else if (ft_strncmp(token,"<<",3) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = HEREDOC;// <<input
		return(0);
	}
	return(1);
}

int	ft_argument_check(char *token, t_token *current_token)
{
	current_token->value = token;
	current_token->type = ARGUMENT;
	return 0;
}
