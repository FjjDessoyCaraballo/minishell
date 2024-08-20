/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/20 14:06:08 by walnaimi         ###   ########.fr       */
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
		//free(current);
		current = next;
	}
}

void free_tokens(t_token *head)
{
    t_token *current = head;
    t_token *next;

    while (current != NULL)
    {
		if(current->next != NULL)
        	next = current->next; // Save the next node

        if(current->value)// Free the dynamically allocated members of the current node
        	free(current->value); // Free the value
        if(current->path)
			free(current->path); // Free the path
        // Free the current node itself
		if(current != NULL)
        	free(current);
        // Move to the next node
        current = next;
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
    if (line_tokenization(data) == 1)// Tokenize and parse the input line
	{
		data->status = 0;//to unstuck the data->status
		return 963;
	}
	free(data->line_read); // 11bytes freed
	// if(data->token != NULL)
	// 	print_tokens(data);
	if (syntax_check(data->token) == 2)// Perform syntax check on the token list
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
	while (head)
	{
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
