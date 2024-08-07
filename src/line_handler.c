/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/07 11:39:21 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Here we are prompting the user to give input with the readline() and
 * tokenizing afterwards. After tokenizing, we are using the tokens to check
 * for invalid inputs. More information in closed issue #19 in the repository.
 */
/*int	sniff_line(t_data *data)
{
	data->line_read = readline("\e[45m[Happy birhday Felipe 🥳]\e[0m ");
	if (!data->line_read)
		return (NULL_LINE);
	if (data->line_read || *data->line_read)
		add_history(data->line_read);
	data->echoed = false;
	line_tokenization(data);
	//a function that calls chunky_checker and check_and_handle_echo
	if (syntax_check(data->token) == 2)
		return (2);
	parse_token(data->token);
	return (0);
}*/
int sniff_line(t_data *data)
{
    data->line_read = readline("\e[45m[Happy birthday Felipe 🥳]\e[0m ");
    if (!data->line_read)
        return NULL_LINE;
    if (*data->line_read)
		add_history(data->line_read);

	data->cmd_ignore = false;
	data->echoed = false;
	data->echo_flag = false;
	
    if (line_tokenization(data) == FAILURE)// Tokenize the input line
		return FAILURE;
    /*t_token *token = data->token;
    while (token != NULL)// Process each token with chunky_checker
    {
        if (chunky_checker(token->value, token, data) == FAILURE)
            return FAILURE;
        token = token->next;
    }
    token = data->token;
	
    while (token != NULL)// Handle echo-specific logic (if needed)
    {
		//print_tokens(data);//debug
        if (check_and_handle_echo(token, &data->token, data->deli, data) == FAILURE)
			return FAILURE;
		
        token = token->next;
    }*/
    if (syntax_check(data->token) == 2)// Perform syntax check on the token list
		return 2;
    parse_token(data->token);// Parse the token
    
    return 0;
}



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
