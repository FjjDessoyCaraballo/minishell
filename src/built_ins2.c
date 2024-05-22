/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:20:14 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/22 14:16:06 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// suscetible to changes after parsing commands from line
// removed get_cwd() and inserted into initialization (check README)
void	shell_cd(t_data *data, t_env *env_ll)
{
    char *temp;
    char *temp2;

	env_ll->dummy = 1;
    temp = NULL;
    temp2 = NULL;
	data->path = getcwd(NULL, 0);
    if (!(getcwd(NULL, 0) == NULL))
        data->path = getcwd(NULL, 0);
    free(data->path);
    temp = ft_strtrim(data->line_read, "cd ");
    temp2 = ft_strjoin("/", temp);
    free(temp);
    temp = ft_strjoin(data->path, temp2);
    free(temp2);
    chdir(temp);
    free(temp);
    data->path = getcwd(NULL, 0);
}
/* This will leak if we don't figure out the array freeing */
void	export(char *cargo, t_env *env_ll)
{
	t_env	*tmp;
	char	**exp_list;
	int		i;
	
	if (!cargo[0])
		print_export(env_ll);
	i = 0;
	exp_list = ft_split(cargo, ' ');
	if (env_ll == NULL)
		env_ll = ft_listnew(exp_list[i++]);
	tmp = env_ll;
	while (exp_list[i])
		ft_listadd_back(&env_ll, ft_listnew(exp_list[i++]));
	env_ll = tmp;
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
// char	*get_cwd(t_env *env_ll)
// {
// 	t_env	*tmp;

// 	tmp = env_ll;
// 	while (env_ll->next != NULL)
// 	{
// 		if (!ft_strncmp("PWD=", env_ll->content, 4))
// 			return (env_ll->content + 4);			
// 		env_ll = env_ll->next;
// 	}
// 	env_ll = tmp;
// 	return (NULL);
// }
	// char	*new_pwd;
	// char	*old_pwd;
	
	// env_ll->dummy = 0;
	// old_pwd = getcwd(NULL, 0);
	// new_pwd = data->line_read + 2;
	// if (!new_pwd[3]) // simple cd no arguments
	// {
	// 	if (chdir(data->home_pwd) == -1)
	// 		ft_putstr_fd(ERR, 2);
	// 	printf("we've reached home\n");
	// 	// export(old_pwd);
	// 	// export(new_pwd);
	// }
	// printf("perm_cwd: %s\npath: %s\nline_read: %s\n", old_pwd, new_pwd, data->line_read);
	// printf("come back later, we are working on it\n");