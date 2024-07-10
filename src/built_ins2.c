/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/10 15:03:30 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// suscetible to changes after parsing commands from line
// removed get_cwd() and inserted into initialization (check README)
void	shell_cd(char *path, t_data *data, t_env *env_ll)
{
	char	*new_pwd;
	char	*curr_pwd;

	if (!path[0])
	{
		chdir(data->home_pwd);
		return ;
	}
	new_pwd = NULL;
	env_ll->dummy = 1;
	data->dummy = 1;
    curr_pwd = getcwd(NULL, 0);
    if (!curr_pwd)
	{
        free(curr_pwd);
		return ;
	}
	path = ft_strtrim(path, "cd ");
	new_pwd = ft_strsjoin(curr_pwd, path, '/');
	free(path);
	chdir(new_pwd);
}
/* This will leak if we don't figure out the array freeing */
void	export(char *cargo, t_env *env_ll)
{
	t_env	*tmp;
	char	**exp_list;
	int		i;
	
	i = 0;
	exp_list = ft_split(cargo, ' ');
	if (env_ll == NULL)
		env_ll = ft_listnew(exp_list[i++]);
	tmp = env_ll;
	while (exp_list[i])
		ft_listadd_back(&env_ll, ft_listnew(exp_list[i++]));
	env_ll = tmp;
	if (!cargo[0])
		print_export(env_ll);
}
// when someone types EXPORT only, it prints all env variables
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented (not really necessary)
void	print_export(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		printf("declare -x ");
		printf("%s\n", env_ll->content);
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	tmp = NULL;
}

/* this function unsets whatever argument given after unset in the command line */
void	unset(char *str, t_env *env_ll)
{
	t_env	*tmp;
	t_env	*del;

	if (!*str || !env_ll || !str)
		return ;
	tmp = env_ll;
	if (!ft_strncmp(str, env_ll->content, ft_strlen(str)))
	{
		del = env_ll;
		// env_ll = env_ll->next;
		free(del);
		return ;
	}
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(str, env_ll->next->content, ft_strlen(str)))
		{
			del = env_ll->next;
			env_ll->next = env_ll->next->next;
			free(del);
			return ;
		}
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	tmp = NULL;
}
