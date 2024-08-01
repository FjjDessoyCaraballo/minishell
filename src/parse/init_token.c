/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:04 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/26 14:00:06 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token* init_token()
{
	t_token *token;

	// t_token *token = (t_token *)malloc(sizeof(t_token));
	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return NULL;
	// ft_bzero(token, sizeof(t_token));
	token->type = UNKNOWN;
	token->value = NULL;
	token->path = NULL;
	token->id = 0;
	token->next = NULL;
	token->prev = NULL;
	token->echo = false;
	return (token);
}
