/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 17:34:11 by fdessoy-         ###   ########.fr       */
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

