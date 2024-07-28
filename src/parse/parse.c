/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:49:16 by walnaimi          #+#    #+#             */
/*   Updated: 2024/07/26 12:55:48 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
int	tokens_parsing(t_token *token)
{
	if (incorrect_syntax(token, PIPE) == FAILURE
		|| incorrect_syntax(token, RED_OUT) == FAILURE
		|| incorrect_syntax(token, RED_IN) == FAILURE
		|| incorrect_syntax(token, HEREDOC) == FAILURE
		|| incorrect_syntax(token, APPEND) == FAILURE)
		return (FAILURE);
	else
		return (SUCCESS);
	
}

int	incorrect_syntax(t_token *token, t_type token_type)
{
	t_token	*head;
	t_type	next_type;
	
	head = token;
	while (head)
	{
		if (head->type == token_type)
		{
			if (head->next != NULL)
			{
				next_type = head->next->type;
				if (next_type == COMMAND || next_type == RED_IN
					|| next_type == PIPE || next_type == RED_OUT
					|| next_type == HEREDOC || next_type == APPEND
					|| next_type == FLAG || next_type == UNKNOWN)
				return (err_pipes(head->value, 2));
			}
		}
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}
/*
int parse(t_data *data)
{
    int status;
    status = 0;

    //status = tokens_parsing(data->token);
    return (status);
}
*/

		// if ((head->type == REDIRECT_IN && next_type == COMMAND)
		// 	|| (head->type == REDIRECT_IN && next_type == REDIRECT_IN)
		// 	|| (head->type == REDIRECT_IN && next_type == REDIRECT_OUT)
		// 	|| (head->type == REDIRECT_IN && next_type == PIPE)
		// 	|| (head->type == REDIRECT_IN && next_type == HEREDOC)
		// 	|| (head->type == REDIRECT_IN && next_type == APPEND)
		// 	|| (head->type == REDIRECT_IN && next_type == FLAG))