/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:10:25 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:10:26 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Initializes a new token.
 *
 * @return A pointer to the newly created token,
 * or NULL if memory allocation fails.
 */
t_token	*init_token(void)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = UNKNOWN;
	token->value = NULL;
	token->path = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->id = 0;
	token->echo = false;
	return (token);
}
