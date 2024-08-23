/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:04 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/20 19:22:31 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
