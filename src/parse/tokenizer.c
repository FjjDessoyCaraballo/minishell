#include "../../includes/minishell.h"

void chunky_checker(char *token,t_token *current_token)
{
	ft_builtin_check(token, current_token);
	if(current_token->prev != NULL && current_token->prev->type == BUILTIN && current_token->id == 1)
	{
		printf("looking for flag\n");
	}
}

void line_tokenization(t_data *data)
{
    char *token;
    const char *delimiters = "  \t\n";
	t_token *first_node = init_token();
	t_token *current_token = first_node;
	t_token *prev_token = NULL;
	int id = 0;

    token = ft_strtok(data->line_read, delimiters);
    while (token != NULL)
    {
		current_token->id = id;
		current_token->prev = prev_token;
		chunky_checker(token, current_token);
        token = ft_strtok(NULL, delimiters);
		if(token != NULL)
		{
			current_token->next = init_token();
			current_token->next->prev = current_token;
			prev_token = current_token;
			current_token = current_token->next;
			id++;
		}
    }
	data->token = first_node;
	//print_tokens(data);
}
