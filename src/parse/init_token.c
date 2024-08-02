/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:04 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/02 11:41:13 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token* init_token(void)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->type = UNKNOWN;
	token->value = NULL;
	token->path = NULL;
	token->id = 0;
	token->next = NULL;
	token->prev = NULL;
	token->echo = false;
	return token;
}
