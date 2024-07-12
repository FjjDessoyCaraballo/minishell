/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/12 14:07:30 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* This part is taking the tokens and taking the argument just after it. */
void	built_ins(t_data *data, t_env **env_ll)
{
	t_token *token = data->token;

	token->value = data->line_read;
	data->home_pwd = get_home((*env_ll));
	if (!ft_strncmp(token->value, "env", 3))
		data->status = print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 3))
		data->status = print_pwd();
	else if (!ft_strncmp(token->value, "exit", 4))
		get_the_hell_out(data, (*env_ll));
	else if (!ft_strncmp(token->value, "echo", 4))
		data->status = yodeling(token->value);
	else if (!ft_strncmp(token->value, "cd", 2))
		data->status = shell_cd(token->value, data);
	else if (!ft_strncmp(token->value, "export", 6))
		data->status = export(token->value + 6, (*env_ll));
	else if (!ft_strncmp(token->value, "unset", 5))
		data->status = unset(token->value + 6, env_ll);
	else
		return ;
	
}
/* The printing of the environment changes in conformity to the use of
export and unset. The command 'env' itself does not take arguments.
Exempli Gratia: $> env || $> pwd (no white spaces or anything like caps)*/
int	print_env(t_env *env_ll)
{
	t_env	*tmp;

	if (!env_ll)
		return (FAILURE);
	tmp = env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	env_ll = tmp;
	return (SUCCESS);
}

int	print_pwd(void)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (FAILURE);
	printf("%s\n", pwd);
	return (SUCCESS);
}
/* This is the exit function, it needs to take, if inputted,
an exit code that was manually inserted after exit */
void	get_the_hell_out(t_data *data, t_env *env_ll)
{
	free_ll(env_ll);
	printf("exit\n");
	exit(data->status);
}

int	yodeling(char *echoes)
{
	if (!ft_strncmp(echoes, "echo -n", 7))
	{
		ft_printf("%s", echoes + 8);
		return (SUCCESS);
	}
	else
	{
		ft_printf("%s\n", echoes + 5);
		return (SUCCESS);
	}
	return (FAILURE);
}