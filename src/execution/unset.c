/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:09:11 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:09:13 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	unset(t_token *token, t_env **env_ll)
{
	t_token	*head;

	if (should_skip_unset(token, env_ll))
		return (SUCCESS);
	head = token->next;
	if (remove_first_env_var(head, env_ll) == SUCCESS)
		return (SUCCESS);
	remove_env_var(head, env_ll);
	return (SUCCESS);
}

int	should_skip_unset(t_token *token, t_env **env_ll)
{
	if (!token || !token->next || !token->next->value || !*env_ll || !env_ll)
		return (1);
	return (0);
}

int	remove_first_env_var(t_token *head, t_env **env_ll)
{
	t_env	*tmp;

	tmp = *env_ll;
	if (!ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
	{
		*env_ll = tmp->next;
		free_env_var(tmp);
		return (SUCCESS);
	}
	return (FAILURE);
}

void	remove_env_var(t_token *head, t_env **env_ll)
{
	t_env	*tmp;
	t_env	*del;

	tmp = *env_ll;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(head->value, tmp->next->content, \
			ft_strlen(head->value)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free_env_var(del);
			return ;
		}
		tmp = tmp->next;
	}
}

void	free_env_var(t_env *env_var)
{
	if (!env_var)
		return ;
	free_null(env_var->key);
	free_null(env_var->value);
	free_null(env_var->content);
	free(env_var);
	env_var = NULL;
}
