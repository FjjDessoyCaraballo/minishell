/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/23 22:56:03 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Prints the tokens in the linked list,
 * including their values, types, and other properties.
 * 
 * @param data A pointer to the t_data structure
 * containing the token linked list.
 * 
 * 
 * @return None
 */
void	print_tokens(t_data *data)
{
	t_token	*token;
	t_token	*last_token;

	const char *type_names[]
	= {
		"UNKNOWN",
		"BUILTIN",
		"COMMAND",
		"ARGUMENT",
		"PIPE",
		"FLAG",
		"ENVVAR",
		"RED_IN",
		"RED_OUT",
		"HEREDOC",
		"APPEND",
	};

	token = data->token;
	last_token = NULL;
	while (token != NULL)
	{
		if (token->value != NULL)
		{
			printf("--------------[%d]--------------\n", token->id);
			printf("token value :[%s]\n", token->value);
			if (token->value[0] == '\0')
				printf("empty string\n");
			printf("token type  :[%s]\n", type_names[token->type]);
			if (token->empty == true)
				printf("empty?      :[%d]\n", token->empty);
			if (token->in_q == true)
				printf("in quotes   :[%d]\n", token->in_q);
			if (token->echo == true)
				printf("echo?       :[%d]\n", token->echo);
			if (token->path != NULL)
				printf("token path  :[%s]\n", token->path);
			printf("\n");
			if (token->next == NULL)
				last_token = token;
		}
		token = token->next;
	}
	printf("#####################################\n");
	token = last_token;
}

// void	print_cmd(char **cmd_a)
// {
// 	int	i;

// 	i = 0;
// 	if (cmd_a == NULL)
// 	{
// 		printf("cmd_a is NULL\n");
// 		return ;
// 	}
// 	while (cmd_a[i])
// 	{
// 		printf("[%s]\n", cmd_a[i]);
// 		i++;
// 	}
// }

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
