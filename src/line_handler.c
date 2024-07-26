/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/07/26 11:25:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sniff_line(t_data *data)
{
	int	status;

	status = 0;
	data->line_read = readline("\e[45m[I can't believe it's not shell]\e[0m ");
	if (!data->line_read)
		return (NULL_LINE);
	//line_tokenization(data);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	line_tokenization(data);
	status = tokens_parsing(data->token);
	return (status);
}

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
		// if ((head->type == REDIRECT_IN && next_type == COMMAND)
		// 	|| (head->type == REDIRECT_IN && next_type == REDIRECT_IN)
		// 	|| (head->type == REDIRECT_IN && next_type == REDIRECT_OUT)
		// 	|| (head->type == REDIRECT_IN && next_type == PIPE)
		// 	|| (head->type == REDIRECT_IN && next_type == HEREDOC)
		// 	|| (head->type == REDIRECT_IN && next_type == APPEND)
		// 	|| (head->type == REDIRECT_IN && next_type == FLAG))