/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/11 17:13:40 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* this whole function is very case sensitive and the pointers given 
as arguments should be replaced immidiately after tokenization */
void	built_ins(t_data *data, t_env **env_ll)
{
	t_token *token = data->token;

	token->value = data->line_read;
	data->home_pwd = get_home((*env_ll));
	if (!ft_strncmp(token->value, "env", 3))
		print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 3))
		print_pwd();
	else if (!ft_strncmp(token->value, "exit", 4))
		get_the_hell_out((*env_ll), ft_atoi(token->value));
	else if (!ft_strncmp(token->value, "echo", 4))
		yodeling(token->value);
	else if (!ft_strncmp(token->value, "cd", 2))
		shell_cd(token->value, data, (*env_ll));
	else if (!ft_strncmp(token->value, "export", 6))
		export(token->value, (*env_ll));
	else if (!ft_strncmp(token->value, "unset", 5))
		unset(token->value, env_ll);
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
	env_ll = tmp;
}

void	print_pwd(void)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
}
/* this might need to be updated after we start piping */
/* when checking for leaks, remember that readline() may leak and its ok*/
void	get_the_hell_out(t_env *env_ll, int num)
{
	free_ll(env_ll);
	exit(num);
}
/* this one will likely change after tokenization */
void	yodeling(char *echoes)
{
	if (!ft_strncmp(echoes, "echo -n", 7))
		ft_printf("%s", echoes + 8);
	else
		ft_printf("%s\n", echoes + 5);
}