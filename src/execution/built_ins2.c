/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/25 22:33:11 by bposa            ###   ########.fr       */
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

/* 
 * export puts variables declared by user in the env
 * */
int	export(t_token *token, t_env **env_ll)
{
	t_token	*tmp_tok;

	if (find_token(token, APPEND) || find_token(token, HERE_DOC)
		|| find_token(token, RED_IN) || find_token(token, RED_OUT))
		return (SUCCESS);
	if (token->next->value == NULL)
		return (print_export(env_ll));
	token = token->next;
	tmp_tok = token;
	while (tmp_tok->next != NULL)
	{
		if (export_util(env_ll, tmp_tok) == FAILURE)
			return (FAILURE);
		tmp_tok = tmp_tok->next;
	}
	return (SUCCESS);
}

int	export_util(t_env **env_ll, t_token *tmp_tok)
{
	int		found;
	t_env	*tmp_ll;
	char	**array;

	found = 0;
	tmp_ll = (*env_ll);
	while (tmp_ll != NULL)
	{
		array = ft_split(tmp_tok->value, '=');
		if (!array)
			return (FAILURE);
		if (!ft_strncmp(tmp_ll->key, array[0], ft_strlen(tmp_ll->key)))
		{
			found = 1;
			free_array(array);
			break ;
		}
		free_array(array);
		tmp_ll = tmp_ll->next;
	}
	if (found && export_util_two(tmp_ll, tmp_tok, array) == FAILURE)
		return (FAILURE);
	else
		ft_listadd_back(env_ll, ft_listnew(tmp_tok->value));
	return (SUCCESS);
}

int	export_util_two(t_env *tmp_ll, t_token *tmp_tok, char **array)
{
	free_null(tmp_ll->value);
	free_null(tmp_ll->key);
	free_null(tmp_ll->content);
	tmp_ll->content = ft_strdup(tmp_tok->value);
	array = ft_split(tmp_tok->value, '=');
	if (!array)
		return (FAILURE);
	tmp_ll->key = ft_strdup(array[0]);
	if (!tmp_ll->key)
	{
		free_array(array);
		return (FAILURE);
	}
	tmp_ll->value = ft_strdup(ft_strchr(tmp_tok->value, '='));
	if (!tmp_ll->value)
	{
		free_array(array);
		free_null(tmp_ll->key);
		return (FAILURE);
	}
	free_array(array);
	return (SUCCESS);
}

/*
 * when someone types EXPORT only, it prints all env variables
 * IN ALPHABETICAL ORDER!!! <- still needs to be implemented
 * (not really necessary)
 * */
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

/*this function unsets whatever argument given after unset in the command line*/
int	unset(t_token *token, t_env **env_ll)
{
	t_env	*tmp;
	t_env	*del;
	t_token	*head;

	head = token;
	if (!head->next->value || !*env_ll || !env_ll)
		return (SUCCESS);
	tmp = *env_ll;
	head = head->next;
	while (!ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
	{
		tmp = tmp->next;
		free_null(tmp->key);
		free_null(tmp->value);
		free_null(tmp->content);
		free(tmp);
		tmp = NULL;
		return (SUCCESS);
	}
	tmp = *env_ll;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(head->value, tmp->next->content,
				ft_strlen(head->value)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free_null(del->key);
			free_null(del->value);
			free_null(del->content);
			free(del);
			del = NULL;
			return (SUCCESS);
		}
		tmp = tmp->next;
	}
	tmp = NULL;
	head = NULL;
	return (SUCCESS);
}
