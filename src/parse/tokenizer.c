/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/15 17:34:17 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int chunky_checker(char *token,t_token *current_token,t_data *data)
{
	if(ft_builtin_check(token, current_token, data->builtins) == SUCCESS)
		return(SUCCESS);
	else if(current_token->prev != NULL && current_token->prev->type == BUILTIN && ft_strcmp(token,"-n") == SUCCESS)
	{
		current_token->type = FLAG;
		current_token->value = "-n";
		return (SUCCESS);
	}
	if(ft_command_check(token, current_token, data->bin) == SUCCESS)
		return(SUCCESS);
	else if(current_token->prev != NULL && (current_token->prev->type == COMMAND || current_token->prev->type == FLAG)
			&& token[0] == '-')
	{
		current_token->type = FLAG;
		current_token->value = ft_strdup(token);
		return(SUCCESS);
	}
	else if(ft_pipe_check(token, current_token) == SUCCESS)
		return(SUCCESS);
	else if(ft_redirect_op_check(token, current_token, data->redirect) == SUCCESS)
		return(SUCCESS);
	else if(ft_argument_check(token, current_token) == SUCCESS)
		return(SUCCESS);
	return(FAILURE);
}

void print_env_ll(t_env *env_ll) 
{
	t_env *temp = env_ll;
	while (temp) 
	{
		printf("%s\n", temp->content);
		temp = temp->next;
	}
}

void line_tokenization(t_data *data, t_env **env_ll)
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
		chunky_checker(token, current_token, data);
		token = ft_strtok(NULL, delimiters);
		//printf("while token\n");
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

	data->cmd_a = tokens_to_array(data->token);
	//print_tokens(data);
	//print_cmd(data->cmd_a);
	//print_env_ll(*env_ll);
	(void)env_ll;
}
