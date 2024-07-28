/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/26 13:59:05 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void print_tokens(t_data *data)
{
    t_token *token = data->token;
	t_token *last_token = NULL;

    // Forward traversal to print all tokens
    while (token != NULL)
    {
        printf("token value:[%s]\n", token->value);
        printf("token type:[%i]\n", token->type);
        printf("id:[%i]\n", token->id);
        if(token->echo == true || data->echoed == true) 
            printf("echo?:[%d]\n",token->echo);
		if(token->path != NULL)
			printf("token path:[%s]\n", token->path);
        if (token->next == NULL) // Stop at the last token
            last_token = token;
        token = token->next;
    }

    // Backward traversal to check prev pointers
	token = last_token;
    while (token != NULL)
    {
       /*printf("prev token value:[%s]\n", token->value);
        printf("prev token type:[%i]\n", token->type);
        printf("prev id:[%i]\n", token->id);*/
        token = token->prev;
    }
}

void print_cmd(char **cmd_a)
{
    if (cmd_a == NULL) {
        printf("cmd_a is NULL\n");
        return;
    }
	int i = 0;
	while(cmd_a[i])
	{
		printf("[%s]\n", cmd_a[i]);
		i++;
	}
}
