/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/15 15:05:37 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_ins(t_data *data, t_env *env_ll)
{
	data->dummy = 0;
	if (!ft_strncmp(data->line_read, "env", 3))
		print_env(env_ll);
	if (!ft_strncmp(data->line_read, "pwd", 3))
		print_pwd(env_ll);
	else
		printf("\n");
}
void	print_env(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content); //extra white last line
		tmp = tmp->next;
	}
}
void	print_pwd(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(env_ll->content, "PWD=", 4))
		{
			printf("%s\n", env_ll->content + 4);
			break ;
		}
		env_ll = env_ll->next;		
	}
	env_ll = tmp;
}