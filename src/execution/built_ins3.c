/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 10:54:31 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/27 11:07:18 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* This is the exit function, it needs to take, if inputted,
an exit code that was manually inserted after exit */
void	get_the_hell_out(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	status = 0;
	free_all_ll(env_ll);
	ft_printf("exit\n");
	if (token->next != NULL && token->next->value != NULL)
	{
		status = ft_atoi(token->next->value);
		free_gang(data);
		exit(status);
	}
	free_gang(data);
	exit(data->status);
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
