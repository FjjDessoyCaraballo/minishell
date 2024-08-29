/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:07:46 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:07:48 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	alphabetical_printer(char **env_array)
{
	char	c;
	int		i;

	i = 0;
	c = 32;
	while (c < 127)
	{
		while (env_array[i])
		{
			if (env_array[i][0] == c)
			{
				printf("declare -x ");
				printf("%s\n", env_array[i]);
			}
			i++;
		}
		i = 0;
		c++;
	}
}

// when someone types EXPORT only, it prints all env variables
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented
// (not really necessary)
int	print_export(t_env **env_ll)
{
	char	**env_array;

	if (!env_ll || !*env_ll)
		return (SUCCESS);
	env_array = env_arr_updater(env_ll);
	if (!env_array)
		return (SUCCESS);
	alphabetical_printer(env_array);
	free_array(env_array);
	return (SUCCESS);
}

int	handle_flag_type(t_token *head)
{
	head = head->next;
	while (head->type == FLAG)
	{
		head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
	}
	while (head->value != NULL)
	{
		while (head->value != NULL && head->type == ARG
			&& head->value[0] == '\0')
			head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
		if (head->type == RED_IN || head->type == RED_OUT
			|| head->type == APPEND || head->type == HEREDOC)
			break ;
		printf("%s", head->value);
		if (head->next->value != NULL && head->next->empty == false)
			printf(" ");
		head = head->next;
	}
	return (SUCCESS);
}

int	handle_arg_type(t_token *head)
{
	head = head->next;
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head->next != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	printf("\n");
	return (SUCCESS);
}
