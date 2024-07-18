/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/17 15:36:31 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// suscetible to changes after parsing commands from line
// removed get_cwd() and inserted into initialization (check README)
int	shell_cd(char *path, t_data *data)
{
	char	*new_pwd;
	char	*curr_pwd;

	if (ft_strlen(path) == 2)
	{
		chdir(data->home_pwd);
		return (SUCCESS);
	}
	new_pwd = NULL;
    curr_pwd = getcwd(NULL, 0);
    if (!curr_pwd)
	{
        free(curr_pwd);
		return (FAILURE);
	}
	path = ft_strtrim(path, "cd ");
	new_pwd = ft_strsjoin(curr_pwd, path, '/');
	free(path);
	chdir(new_pwd);
	return (SUCCESS);
}
/* This will leak if we don't figure out the array freeing */
int	export(char *cargo, t_env *env_ll)
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
	{
		print_export(env_ll);
		return (SUCCESS);
	}
	return (SUCCESS);
}
// when someone types EXPORT only, it prints all env variables
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented (not really necessary)
int	print_export(t_env *env_ll)
{
	t_env	*tmp;

	if (!env_ll)
		return (SUCCESS);
	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		printf("declare -x ");
		printf("%s\n", env_ll->content);
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	tmp = NULL;
	return (SUCCESS);
}

/* this function unsets whatever argument given after unset in the command line */
int	unset(char *str, t_env **env_ll)
{
	t_env	*tmp;
	t_env	*del;

	if (!*str || !*env_ll || !str || !env_ll)
		return (SUCCESS);
	tmp = *env_ll;
	if (!ft_strncmp(str, tmp->content, ft_strlen(str)))
	{
		*env_ll = tmp->next;
		free(tmp);
		return (SUCCESS);
	}
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(str, tmp->next->content, ft_strlen(str)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free(del);
			return (SUCCESS);
		}
		tmp = tmp->next;
	}
	*env_ll = tmp;
	tmp = NULL;
	return (SUCCESS);
}
