#include "../../includes/minishell.h"

t_token* init_token()
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->type = ARGUMENT;
	token->value = NULL;
	token->id = 0;
	token->next = NULL;
	return token;
}
