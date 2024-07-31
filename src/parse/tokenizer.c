/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/31 22:48:42 by walnaimi         ###   ########.fr       */
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
	else if(data->echoed == false && (ft_command_check(token, current_token, data) == SUCCESS))	//(current_token->id == 0 || current_token->prev->type == PIPE) && 
		return SUCCESS;
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

char *concatenate_echo_args(const char *delimiters, t_data *data) 
{
    char *concatenated_args = NULL;
    char *token;
    int first_token = 1;
	(void)(data);
	

    while ((token = ft_strtok(NULL, delimiters, data)) != NULL)
    {
		if (data->error == 4)
		{
			printf("Tokenization error within concatenate_echo_args: unmatched quote detected.\n");
            return NULL;
		}
        char *temp;

        // Handle environment variables
        /*if (ft_strcmp(token, "$?") == 0)
        {
            char *env_status = ft_itoa(data->status);
            if (concatenated_args)
            {
                temp = ft_strjoin(concatenated_args, " ");
                free(concatenated_args);
                concatenated_args = ft_strjoin(temp, env_status);
                free(temp);
            }
            else
            {
                concatenated_args = ft_strdup(env_status);
            }
            free(env_status);
        }
        else if (token[0] == '$') 
        {
            char *env_value = ft_getenv(token + 1, data->envll);
            if (env_value)
            {
                if (concatenated_args)
                {
                    temp = ft_strjoin(concatenated_args, " ");
                    free(concatenated_args);
                    concatenated_args = ft_strjoin(temp, env_value);
                    free(temp);
                }
                else
                {
                    concatenated_args = ft_strdup(env_value);
                }
            }
        }*/
        //else
        //{
            if (concatenated_args)
            {
                // Only add a space if it's not the first token
                if (!first_token)
                {
                    temp = ft_strjoin(concatenated_args, " ");
                    free(concatenated_args);
                    concatenated_args = ft_strjoin(temp, token);
                    free(temp);
                }
                else
                {
                    temp = ft_strdup(token);
                    free(concatenated_args);
                    concatenated_args = temp;
                }
            }
            else
            {
                concatenated_args = ft_strdup(token);
            }
        //}

        first_token = 0; // After the first token, subsequent tokens should be prefixed with a space
    }

    return concatenated_args;
}


void echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data)
{
    char *concatenated_args = concatenate_echo_args(delimiters, data);
	if (concatenated_args == NULL && data->error == 4)
	{
		// Handle error
        printf("error: unmatched quote detected.\n");
        return;
	}
    current_token->next = init_token();
    current_token->next->prev = current_token;
    *prev_token = current_token;
    current_token = current_token->next;
    current_token->type = ARGUMENT;
    current_token->value = concatenated_args;
}

int line_tokenization(t_data *data)
{
    char *token;
    const char *delimiters = "  \t\n";
    t_token *first_node = init_token();
    t_token *current_token = first_node;
    t_token *prev_token = NULL;
    int id = 0;
    data->error = 0;

    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    data->builtins = builtins;
    char *redirect[] = {">", ">>", "<", "<<", NULL};
    data->redirect = redirect;

    token = ft_strtok(data->line_read, delimiters, data);
    if (data->error == 4)
    {
        // Handle the error appropriately, e.g., clean up and return failure
        printf("Tokenization error: unmatched quote detected. 221\n");
        data->error = 0;
        printf("%i\n",data->error);
        return FAILURE;
    }

    while (token != NULL && data->error != 4)
    {
        current_token->id = id;
        current_token->prev = prev_token;
        chunky_checker(token, current_token, data); //FAILURE
            //return FAILURE;
        if (data->echoed && current_token->type == BUILTIN && data->error != 4)
        {
            echoing(current_token, &prev_token, delimiters, data);
            if (data->error == 4)
            {
                printf("Echoing error: unmatched quote detected after echoing.\n");
                data->error = 0;
                return FAILURE;
            }
            break;
        }

        token = ft_strtok(NULL, delimiters, data);
        if (data->error == 4)
        {
            // Handle the error appropriately, e.g., clean up and return failure
            printf("Tokenization error: unmatched quote detected during tokenization.\n");
            return FAILURE;
        }

        if (token != NULL && data->error != 4)
        {
            current_token->next = init_token();
            current_token->next->prev = current_token;
            prev_token = current_token;
            current_token = current_token->next;
            id++;
        }
    }
    data->token = first_node;
    print_tokens(data); // debug
    return SUCCESS;
}
