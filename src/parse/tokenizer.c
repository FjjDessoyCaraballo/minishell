#include "../../includes/minishell.h"

void	ft_builtin_check(char* token, t_token* current_token)
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
		printf("@@@@  %d  @@@@\n",current_token->id);
	}

	else
	{
		current_token->value = ft_strdup(token);
		current_token->type = UNKNOWN;
	}
}

void line_tokenization(t_data *data)
{
    char *token;
    const char *delimiters = "  \t\n";
	t_token *first_node = init_token();
	t_token *current_token = first_node;
	int id = 0;

    token = ft_strtok(data->line_read, delimiters);
    while (token != NULL)
    {
		current_token->id = id;
		ft_builtin_check(token, current_token);
        token = ft_strtok(NULL, delimiters);
		if(token != NULL)
		{
			current_token->next = init_token();
			current_token = current_token->next;
			id++;
		}
    }
	data->token = first_node;
	print_tokens(data);
}
