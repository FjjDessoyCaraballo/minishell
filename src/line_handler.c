/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/21 00:52:13 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_path(t_token *head)
{
	t_token *current = head;
	t_token *next;

	while (current != NULL)
	{
		next = current->next;
		if (current->path)
			free(current->path);
		current = next;
	}
}

void free_tokens(t_token *head)
{
    t_token *tmp;

    while (head != NULL)
    {
		tmp = head;
        if (tmp->value)
        	free(tmp->value);
        if(tmp->path)
			free(tmp->path);
		head = head->next;
		free_null(tmp);
    }
}

int total_env_len(t_env *head)
{
    int total_length = 0;
    t_env *current = head;

    // Traverse the list from the head to the end
    while (current != NULL)
    {
        if (current->value) // Check if value is not NULL
            total_length += strlen(current->value);
        
        // Move to the next node
        current = current->next;
    }

    return total_length;
}

void setup(t_data *data)
{
    data->deli = "  \t\n";
    data->id = 0;
    data->vtoken = 0;
    data->cmd_ignore = false;
    data->echoed = false;
    data->echo_flag = false;
	data->here_doc = false;
	data->redirections = false;
	data->piped = false;
	data->env_len = total_env_len(data->envll);
}

/**
 * Here we are prompting the user to give input with the readline() and
 * tokenizing afterwards. After tokenizing, we are using the tokens to check
 * for invalid inputs. More information in closed issue #19 in the repository.
 */
int sniff_line(t_data *data)
{
    data->line_read = readline("\e[45m[I can't believe this is not shell]\e[0m ");
    if (!data->line_read)
        return NULL_LINE;
    if (*data->line_read)
		add_history(data->line_read);
	setup(data);
    if (line_tokenization(data) == FAILURE)
	{
		free_gang(data);
		free(data->line_read);
		free(data->new_str);
		free(data->first_node);
		//free(data->line_read);
		data->status = 0;
		return 963;
	}
		free(data->line_read);
    if (syntax_check(data->token) == 2)
		return 2;	
	data->piped = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}

//
/**
 * As we run through the tokens (nodes in a linked list) we check
 * if the next token type is valid for the token type that the user
 * inputted previously. Examples below:
 * "%> |||"
 * "%> syntax error near unexpected token `||'"
 */
int	syntax_check(t_token *token)
{
	if (incorrect_syntax(token, PIPE) == 2
		|| incorrect_syntax(token, RED_OUT) == 2
		|| incorrect_syntax(token, RED_IN) == 2
		|| incorrect_syntax(token, HEREDOC) == 2
		|| incorrect_syntax(token, APPEND) == 2)
		return (2);
	else
		return (SUCCESS);
}

/**
 * incorrect_syntax() checks for specific operators and checks if they're
 * being repeated by the users.
 * 
 * RETURN VALUES: Upon success, it returns 0. If it fails, it returns 2.
 */

int	incorrect_syntax(t_token *token, t_type token_type)
{
	t_token	*head;

	head = token;
	while (head && head->next != NULL && head->in_quotes == false)
	{
		//printf("head->value: %s\nhead->in_quotes: %d\n", head->value, head->in_quotes);
		if (head->next != NULL)
		{
			if ((head->type == token_type && head->next->type == token_type)
				|| (head->type == token_type && head->next->type == RED_IN)
				|| (head->type == token_type && head->next->type == RED_OUT)
				|| (head->type == token_type && head->next->type == HEREDOC)
				|| (head->type == token_type && head->next->type == APPEND)
				|| (head->type == token_type && head->next->type == FLAG))
					return (err_msg(head->value, SYNTAX, 2));
		}
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}
