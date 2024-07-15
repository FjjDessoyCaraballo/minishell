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

int		ft_command_check(char *token, t_token *current_token, char *bin_paths)
{
	if (current_token->type != UNKNOWN)
		return FAILURE;

	char **paths = ft_split(bin_paths, ':');
	char *executable_path = loop_path_for_binary(token, paths);
	if (executable_path != NULL)
	{
		current_token->value = executable_path;
		current_token->type = COMMAND;
		return(SUCCESS);
	}
	else
	{
		int i = 0;
		while(paths[i])
			free(paths[i++]);
		free(paths);
		return(FAILURE);
	}
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
