/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 10:39:29 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int env_var(char *token, t_token *current_token, t_data *data)
{
	if (ft_strcmp(token, "$?") == 0)
	{
		current_token->type = ENVVAR;
		current_token->value = ft_itoa(data->status);
		//printf("%s\n", current_token->value);
		return SUCCESS;
	}
	else if(token[0] == '$')
	{
		char *env_value = ft_getenv(token + 1, data->envll);
			if (env_value)
		{
			current_token->type = ARGUMENT;
			current_token->value = ft_strdup(env_value);
			return SUCCESS;
		}
			printf("\n");
			return FAILURE;
	}
	return FAILURE;
}

int chunky_checker(char *token,t_token *current_token,t_data *data)
{
	if (env_var(token, current_token, data) == SUCCESS)
		return SUCCESS;
	else if(ft_builtin_check(token, current_token, data->builtins) == SUCCESS) //current_token->id == 0 && 
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
		if(current_token->id == 0)
			return (FAILURE);
		else if(data->echoed == true)
			current_token->echo = true;
		return SUCCESS;
	}
	else
		return(FAILURE);
}

char *concatenate_echo_args(t_token *current_token, const char *delimiters, t_data *data) {
    char *concatenated_args = NULL;
    char *token;
    int first_token = 1;

    while ((token = ft_strtok(NULL, delimiters, data, current_token)) != NULL) {
        if (data->error == 4)
            return NULL;
        char *temp;

        // Expand environment variables within the token
        char *expanded_token = expand_env_variables(token, data);

        if (concatenated_args) {
            // Only add a space if it's not the first token
            if (!first_token) {
                temp = ft_strjoin(concatenated_args, " ");
                free(concatenated_args);
                concatenated_args = ft_strjoin(temp, expanded_token);
                free(temp);
            } else {
                temp = ft_strdup(expanded_token);
                free(concatenated_args);
                concatenated_args = temp;
            }
        } else {
            concatenated_args = ft_strdup(expanded_token);
        }

        free(expanded_token); // Free the expanded token
        first_token = 0; // After the first token, subsequent tokens should be prefixed with a space
    }

    return concatenated_args;
}

void echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data)
{
    char *concatenated_args = concatenate_echo_args(current_token, delimiters, data);
    if (concatenated_args == NULL && data->error == 4)
        return;
    //printf("this catted arg is :%s\n", concatenated_args);//debug
    current_token->next = init_token();
    current_token->next->prev = current_token;
    *prev_token = current_token;
    current_token = current_token->next;
    current_token->type = ARGUMENT;
    current_token->value = concatenated_args;
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

int line_tokenization(t_data *data)
{
    t_token *first_node = init_token();
    t_token *current_token = first_node;
    t_token *prev_token = NULL;
    
    setup(data);
    data->vtoken = ft_strtok(data->line_read, data->deli, data, current_token);
    while (data->vtoken != NULL && data->error != 4)
    {
        current_token->id = data->id;
        current_token->prev = prev_token;
        if(chunky_checker(data->vtoken, current_token, data) == FAILURE)
            return FAILURE;
        if (check_and_handle_echo(current_token, &prev_token, data->deli, data) == FAILURE)
            return FAILURE;
        data->vtoken = ft_strtok(NULL, data->deli, data, current_token);
        if (data->error == 4)
            return FAILURE;
        if (data->vtoken != NULL && data->error != 4)
        {
            current_token = create_and_link_next_token(current_token, data);
            prev_token = current_token->prev;
        }
    }
    data->token = first_node;
    return SUCCESS;
}   

//print_tokens(data); // debug