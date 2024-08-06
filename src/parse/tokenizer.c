/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/06 16:27:57 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int chunky_checker(char *token,t_token *current_token,t_data *data)
{
	if(ft_builtin_check(token, current_token, data->builtins) == SUCCESS) //current_token->id == 0 && 
	{
		if (ft_strcmp(current_token->value, "echo") == SUCCESS)
		{
			data->echoed = true;
			data->echo_flag = false;
		}
		return(SUCCESS);
	}
	else if(data->echo_flag)
	{
		current_token->type = FLAG;
		current_token->value = "-n";
		return (SUCCESS);
	}
    else if(current_token->prev != NULL && (current_token->prev->type == COMMAND || current_token->prev->type == FLAG)
		&& token[0] == '-')
	{
		current_token->type = FLAG;
		current_token->value = ft_strdup(token);
		return(SUCCESS);
	}
	else if(ft_pipe_check(token, current_token) == SUCCESS)
	{
		current_token->echo = false;
		data->echoed = false;
		return SUCCESS;
	}
	else if(ft_redirect_op_check(token, current_token, data->redirect) == SUCCESS)
	{
		current_token->echo = false;
		data->echoed = false;
		return SUCCESS;
	}
    
	else if(data->echoed == false && (ft_command_check(token, current_token, data) == SUCCESS))	//(current_token->id == 0 || current_token->prev->type == PIPE) && 
		{
            return SUCCESS;
        }

	else if(ft_argument_check(token, current_token) == SUCCESS)
	{
		if(current_token->id == 1 && current_token->prev->type == ARGUMENT)
        {
            printf("perhaps here\n");
			return (FAILURE);
        }
		else if(data->echoed == true)
			current_token->echo = true;
		return SUCCESS;
	}
	else
    {
        printf("couldn't find type for token\n");
		return(FAILURE);
    }
    return(SUCCESS);
}

int check_and_handle_echo(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data) 
{
    if (data->echoed && current_token->type == BUILTIN && data->error != 4) 
    {
        echoing(current_token, prev_token, delimiters, data);
        if (data->error == 4) 
        {
            data->error = 0;
            return FAILURE;
        }
        if (current_token->next->value == NULL) 
        {
            printf("\n");
            return FAILURE;
        }
        return SUCCESS; // Indicating to break the loop
    }
    return SUCCESS; // Indicating to continue the loop
}

static void setup(t_data *data)
{
    static char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    static char *redirect[] = {">", ">>", "<", "<<", NULL};
    data->deli = "  \t\n";
    data->error = 0;
    data->builtins = builtins;
    data->redirect = redirect;
    data->id = 0;
    data->vtoken = 0;
}

t_token *create_and_link_next_token(t_token *current_token, t_data *data)
{
    // Create and initialize the next token
    t_token *new_token = init_token();
    current_token->next = new_token;
    new_token->prev = current_token;
    // Increment the id in the data structure
    data->id++;
    // Return the newly created token as the new current_token
    return new_token;
}

void expand_token_if_needed(t_token *current_token, t_data *data)
{
    // Check if the token needs expansion and if it has a valid value
    //printf("data->quote: %d\ncurrent_token->expand: %d\ncurrent_token->value:%s\n", data->quote, current_token->expand, current_token->value);
    if (current_token->expand && current_token->value && data->quote != 1)//SOLTUION TO EXPAND OR NOT TO EXPAND
    {
        // Expand environment variables
        char *expanded_value = expand_env_variables(current_token->value, data);
        if (expanded_value)
        {
            // Free the old value if it exists
            if (current_token->value)
                free(current_token->value);
            // Update the token's value with the expanded value
            current_token->value = expanded_value;
        }
        else
        {
            // Handle expansion failure if needed
            // For now, just set current_token->value to a default or handle the error
            current_token->value = strdup("");  // Assign an empty string or handle as needed
        }
    }
}

/*
 * This function performs tokenization on a line of input.
 * It initializes the first token node, sets up the tokenization process,
 * and iteratively processes each token until there are no more tokens or an error occurs.
 *
 * @param data A pointer to the main data structure containing information about the shell.
 *
 * @return Returns SUCCESS if the tokenization is successful, or FAILURE if an error occurs.
 *
 */

int line_tokenization(t_data *data)
{
    t_token *first_node = init_token();
    t_token *current_token = first_node;
    t_token *prev_token = NULL;

    setup(data);
    //printf("data->line_read: %s\n\n", data->line_read);
    data->vtoken = ft_strtok(data->line_read, data->deli, data, current_token);

    while (data->vtoken != NULL && data->status != 4)
    {
        current_token->id = data->id;
        current_token->prev = prev_token;
        current_token->value = ft_strdup(data->vtoken);
        //expand_token_if_needed(current_token, data);
        if (chunky_checker(current_token->value, current_token, data) == FAILURE)
            return FAILURE;
        if (check_and_handle_echo(current_token, &prev_token, data->deli, data) == FAILURE)
            return FAILURE;
        data->vtoken = ft_strtok(NULL, data->deli, data, current_token);
        if (data->status == 4)
            return FAILURE;
        if (data->vtoken != NULL && data->status != 4)
        {
            current_token = create_and_link_next_token(current_token, data);
            prev_token = current_token->prev;
        }
    }
    data->token = first_node;
    //print_tokens(data);//debug
    return SUCCESS;
}