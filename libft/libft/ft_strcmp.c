/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/built_ins2.c
/*   Created: 2024/05/24 09:48:26 by lstorey           #+#    #+#             */
/*   Updated: 2024/05/24 09:49:35 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// susceptible to changes after parsing commands from line
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
    if (curr_pwd == NULL)
        free(curr_pwd);
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
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented
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
}
=======
/*   Created: 2024/07/11 14:06:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/11 14:18:06 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
>>>>>>> main:libft/libft/ft_strcmp.c
