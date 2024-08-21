/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/21 14:47:30 by walnaimi         ###   ########.fr       */
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
		{
        	free(tmp->value);
			tmp->value = NULL;
		}
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
    data->tok_res = 0;
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
    if (line_tokenization(data) == 1)
	{
		free(data->line_read);
		/*free(data->tok_str);
		data->tok_str = NULL;*/
		free_gang(data);
		free_tokens(data->first_node);
		data->status = 0;
		return 963;
	}
	// if(data->token != NULL)
	// 	print_tokens(data);
	free(data->line_read);
	data->piped = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}