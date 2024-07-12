#include "../../includes/minishell.h"

void	ft_builtin_check(char *token, t_token *current_token)
{
	if (ft_strcmp(token,"echo") == 0 || ft_strcmp(token, "cd") == 0 ||
			ft_strcmp(token, "pwd") == 0 || ft_strcmp(token, "export") == 0 ||
			ft_strcmp(token, "unset") == 0 || ft_strcmp(token, "env") == 0 ||
			ft_strcmp(token, "exit") == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = BUILTIN;
	}

	else if (current_token->id == 1 && ft_strcmp(token, "-n") == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = FLAG;
	}

	else
	{
		current_token->value = ft_strdup(token);
		current_token->type = UNKNOWN;
	}
}
