/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:11:30 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:11:32 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_exit_code == HEREDOC_SIG)
			handler(SIGQUIT);
		else if (g_exit_code == EXEC_SIG)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sig == SIGQUIT)
	{
		printf("\n");
		clear_history();
		signal(sig, SIG_DFL);
		kill(getpid(), sig);
	}
}

char	*get_home(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(env_ll->content, "HOME=", 5))
			return (env_ll->content + 5);
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	return (NULL);
}

void	ft_listadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst)
	{
		if (*lst)
		{
			last = ft_list_last(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

t_env	*ft_list_last(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	free_ll(t_env *env_ll)
{
	t_env	*tmp;

	while (env_ll != NULL)
	{
		tmp = env_ll;
		free(env_ll->key);
		free(env_ll->value);
		free(env_ll->content);
		env_ll = env_ll->next;
		free(tmp);
		tmp = NULL;
	}
}
