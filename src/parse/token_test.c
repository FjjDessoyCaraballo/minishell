#include "../../includes/minishell.h"

/*
 * instruction how to use the tokens:
 *
 * -it is best to have data to be the parameter.
 * -we make a local copy with "t_token *token = data->token;"
 * -use the local copy "token" to go through the linked list "while (token != NULL)"
 *		-in the while loop it will preform the function (in this case printf).
 *		-then we move to the next node of the token linked list with token = token->next.
 */
void	print_tokens(t_data *data)
{
	t_token *token = data->token;
	while (token != NULL)
	{
		printf("token value:[%s]\ntoken type:[%i]\nid:[%i]\n\n", token->value, token->type, token->id);
		token = token->next;
	}
}
