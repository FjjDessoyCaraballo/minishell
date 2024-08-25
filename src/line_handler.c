/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 01:33:29 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	total_env_len(t_env *head)
{
	int		total_length;
	t_env	*current;

	total_length = 0;
	current = head;
	while (current != NULL)
	{
		if (current->value)
			total_length += strlen(current->value);
		current = current->next;
	}
	return (total_length);
}

void	setup(t_data *data)
{
	data->deli = "  \t\n";
	data->id = 0;
	data->tok_res = 0;
	data->ignore_cmd = false;
	data->echoed = false;
	data->echo_flag = false;
	data->redirections = false;
	data->piped = false;
	data->env_len = total_env_len(data->envll);
	if (data->status == 963)
		data->status = 2;
}

void	check_and_mark_empty_tokens(t_token *first_token)
{
	t_token	*current_token;

	current_token = first_token;
	while (current_token != NULL)
	{
		if (current_token->value == NULL || current_token->value[0] == '\0')
		{
			current_token->empty = true;
		}
		else
		{
			current_token->empty = false;
		}
		current_token = current_token->next;
	}
}

/**
 * Here we are prompting the user to give input with the readline() and
 * tokenizing afterwards. After tokenizing, we are using the tokens to check
 * for invalid inputs. More information in closed issue #19 in the repository.
 */
int	sniff_line(t_data *data)
{
	data->line_read = readline("\e[1;45m[I can't believe this is"
			" not shell]\e[0m ");
	if (!data->line_read)
		return (NULL_LINE);
	if (*data->line_read)
		add_history(data->line_read);
	setup(data);
	line_tokenization(data);
	if (data->status == 963)
	{
		free(data->line_read);
		return (963);
	}
	data->status = 0;
	check_and_mark_empty_tokens(data->token);
	free(data->line_read);
	if (syntax_check(data->token) == FAILURE)
	{
		data->status = 2;
		return (2);
	}
	data->piped = false;
	data->heredoc_exist = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}
