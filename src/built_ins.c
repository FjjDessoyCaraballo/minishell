/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/20 14:32:58 by fdessoy-         ###   ########.fr       */
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
	if (!ft_strncmp(data->line_read, "exit", 4))
		get_the_hell_out(env_ll);
	if (!ft_strncmp(data->line_read, "echo", 4))
		yodeling(data->line_read);
	if (!ft_strncmp(data->line_read, "cd", 2))
		shell_cd(data, env_ll);
	else
		return ;
}
/* all of the builtins depend on simple syntax right now
Exempli Gratia: $> env || $> pwd (no white spaces or anything like caps)*/
void	print_env(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content);
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
/* this might need to be updated after we start piping */
/* when checking for leaks, remember that readline() may leak and its ok*/
void	get_the_hell_out(t_env *env_ll)
{
	free_ll(env_ll);
	exit(0);
}
/* this one will likely change after tokenization */
void	yodeling(char *echoes)
{
	if (!ft_strncmp(echoes, "echo -n", 7))
		ft_printf("%s", echoes + 8);
	else
		ft_printf("%s\n", echoes + 5);
}