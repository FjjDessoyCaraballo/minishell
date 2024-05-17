/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/17 19:08:14 by fdessoy-         ###   ########.fr       */
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
/* echo -n has an extra space in the output. Needs fixing.*/
/* TEMPORARY FIX: splitting echo with flag */
/* The temporary fix looks too good, can we keep it? */
/* altho it needs to handle spaces at the end of strings */
void	yodeling(char *echoes)
{
	static char	**echo_nnl = NULL;
	int			i;
	int			flag = 0;
/*we should make a flag for whenever we have whitespace at the
end of the string. Otherwise we are printing everything with the
white space missing.*/
	
	i = 0;
	if (!ft_strncmp(echoes, "echo -n", 7))
	{
		echo_nnl = ft_split(echoes, ' ');
		while (echo_nnl[i + 2])
		{
			ft_printf("%s", echo_nnl[i + 2]);
			if (echo_nnl[i + 3] != NULL)
				ft_printf(" ");	
			i++;
		}
		free_array(echo_nnl);
	}
	else
		ft_printf("%s\n", echoes + 5);
}