/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 18:22:06 by walnaimi         ###   ########.fr       */
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
/*
void print_env_ll(t_data *data) 
{
	t_env *temp = data->envll;
	while (temp) 
	{
		printf("%s\n", temp->content);
		temp = temp->next;
	}
}
*/

char *concatenate_echo_args(const char *delimiters, t_data *data) 
{
    char *concatenated_args = ft_strdup("");
    char *token;
	//int first_token = 1;
    while ((token = ft_strtok(NULL, delimiters)) != NULL)
	{
        // Handle environment variables
        if (ft_strcmp(token, "$?") == 0)
		{
            char *env_status = ft_itoa(data->status);
            char *temp = ft_strjoin(concatenated_args, " ");
            //free(concatenated_args);
            concatenated_args = ft_strjoin(temp, env_status);
            //free(temp);
            //free(env_status);
        }
		else if (token[0] == '$') 
		{
            char *env_value = ft_getenv(token + 1, data->envll);
            if (env_value)
			{
                char *temp = ft_strjoin(concatenated_args, " ");
                free(concatenated_args);
                concatenated_args = ft_strjoin(temp, env_value);
                free(temp);
            }
        }
		else
		{
            char *temp = ft_strjoin(concatenated_args, " ");
            free(concatenated_args);
            concatenated_args = ft_strjoin(temp, token);
            free(temp);
        }
    }
	return concatenated_args;
}

void echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data)
{
    char *concatenated_args = concatenate_echo_args(delimiters, data);
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

	char *builtins[] = {"echo","cd","pwd","export","unset","env","exit",NULL};
	data->builtins = builtins;
	char *redirect[] = {">",">>","<","<<", NULL};
	data->redirect = redirect;

    token = ft_strtok(data->line_read, delimiters);
    while (token != NULL)
    {
		current_token->id = id;
		current_token->prev = prev_token;
		if(chunky_checker(token, current_token, data) == FAILURE)
			return FAILURE;

		if(data->echoed && current_token->type == BUILTIN)
		{
			echoing(current_token, &prev_token, delimiters, data);
			break;
		}
		
		token = ft_strtok(NULL, delimiters);
		if(token != NULL)
		{
			current_token->next = init_token();
			current_token->next->prev = current_token;
			prev_token = current_token;
			current_token = current_token->next;
			id++;
		}
	}
	data->token = first_node;
    print_tokens(data);
	return SUCCESS;
}
