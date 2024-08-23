/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 22:21:18 by bposa             #+#    #+#             */
/*   Updated: 2024/08/23 16:49:58 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_my_boi(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

void	free_path(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	next = NULL;
	while (current != NULL)
	{
		next = current->next;
		if (current->path)
			free(current->path);
		current = next;
	}
}

void	free_gang(t_data *data)
{
	if (data->tok_res != NULL)
		free_null(data->tok_res);
	if (data->temp_str != NULL)
		free_null(data->temp_str);
	if (data->new_str != NULL)
		free_null(data->new_str);
	free_tokens(data->token);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = NULL;
	while (head != NULL)
	{
		tmp = head;
		if (tmp->value)
		{
			free_null(tmp->value);
			tmp->value = NULL;
		}
		if (tmp->path)
			free(tmp->path);
		head = head->next;
		free_null(tmp);
	}
}
