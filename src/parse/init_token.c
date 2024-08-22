/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:04 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 15:08:05 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Initializes a new token.
 *
 * @return A pointer to the newly created token,
 * or NULL if memory allocation fails.
 */
t_token* init_token(void)
{
	t_token *token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return NULL;
	token->type = UNKNOWN;
	token->value = NULL;
	token->path = NULL;
	token->id = 0;
	token->next = NULL;
	token->prev = NULL;
	token->echo = false;
	return (token);
}
