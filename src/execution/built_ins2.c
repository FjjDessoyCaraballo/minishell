/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 17:26:49 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void alphabetical_printer(char **env_array)
{
    char	c;
	int		i;
	
	i = 0;
    c = 32;
    while (c < 127)
    {
        while (env_array[i])
        {
			//name = tmp->key;
            if (env_array[i][0] == c)
            {
                printf("declare -x ");
				printf("%s\n", env_array[i]);
            }
			i++;
        }
		i = 0;
        c++;
    }
}

// suscetible to changes after parsing commands from line
// removed get_cwd() and inserted into initialization (check README)
int	shell_cd(t_token *token, t_data *data)
{
	static char	*new_pwd;
	char		*curr_pwd;

	if (token->next->value == NULL)
	{
		chdir(data->home_pwd);
		return (SUCCESS);
	}
	token = token->next;
	if (ft_strchr(token->value, '/') == NULL)
	{
		curr_pwd = getcwd(NULL, 0);
		if (!curr_pwd)
		{
			free_null(curr_pwd);
			chdir(data->home_pwd);
			return (err_msg(token->value, "Nothingness ahead\n", 1));
		}
		new_pwd = ft_strsjoin(curr_pwd, token->value, '/');
		if (!new_pwd)
			return (err_msg(NULL, MALLOC, -1));
		if (chdir(new_pwd) < 0)
		{
			free_null(new_pwd);
			free_null(curr_pwd);
			return (err_msg(token->value, FILE_ERROR, 1));
		}
		free_null(curr_pwd);
		free_null(new_pwd);
	}
	else
	{
		if (chdir(token->value) < 0)
			return (err_msg(token->value, FILE_ERROR, 1));
	}
	return (SUCCESS);
}

/* export puts variables declared by user in the env */
int	export(t_token *token, t_env **env_ll, int i)
{
	t_env	*tmp;
	char	**exp_list;
	int		count;
	t_token	*head;

	head = token;
	count = 0;
	if (!head->next->value)
	{
		print_export(env_ll);
		return (SUCCESS);
	}
	head = token->next;
	if(head->value[0] >= '0' && head->value[0] <= '9')
		return(err_msg(head->value,ERR_EXP,FAILURE));
	tmp = (*env_ll);
	while (head != NULL)
	{
		while (tmp)
		{
			if (ft_strncmp(tmp->key, head->value, ft_strlen(tmp->key)) == 0)
			{
				// free(tmp->content);
				// free(tmp->value);
				tmp->content = ft_strdup(head->value);
				tmp->value = ft_substr(head->value, ft_strlen(tmp->key) + 1, ft_strlen(head->value) - ft_strlen(tmp->key)); 
				return (SUCCESS);
			}
			tmp = tmp->next;
		}
		
		head = head->next;
		count++;
	}
	exp_list = (char **)malloc(sizeof(char *) * (count + 1));
	if (!exp_list)
		return (FAILURE);
	head = token->next;
	i = 0;
	while (head != NULL)
	{
		if (head->value != NULL)
			exp_list[i++] = ft_strdup(head->value);
		head = head->next;
	}
	exp_list[i] = NULL;
	i = 0;
	tmp = (*env_ll);
	if (!*env_ll)
		(*env_ll) = ft_listnew(exp_list[i++]);
	i = 0;
	while (exp_list[i])
		ft_listadd_back(env_ll, ft_listnew(exp_list[i++]));
	(*env_ll) = tmp;
	tmp = NULL;
	//free_array(exp_list);
	return (SUCCESS);
}

// when someone types EXPORT only, it prints all env variables
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented (not really necessary)
int	print_export(t_env **env_ll)
{
	char	**env_array;

	if (!env_ll || !*env_ll)
		return (SUCCESS);
	env_array = env_arr_updater(env_ll);
	if (!env_array)
		return (SUCCESS);
	alphabetical_printer(env_array);
	free_array(env_array);
	return (SUCCESS);
}

/* this function unsets whatever argument given after unset in the command line */
int	unset(t_token *token, t_env **env_ll)
{
	t_env	*tmp;
	t_env	*del;
	t_token	*head;

	head = token;
	if (!head->next->value || !*env_ll || !env_ll)
		return (SUCCESS);
	tmp = *env_ll;
	head = head->next;
	while (!ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
	{
		tmp = tmp->next;
		free_null(tmp->key);
		free_null(tmp->value);
		free(tmp);
		tmp = NULL;
		return (SUCCESS);
	}
	tmp = *env_ll;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(head->value, tmp->next->content,
				ft_strlen(head->value)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free_null(del->key);
			free_null(del->value);
			free(del);
			del = NULL;
			return (SUCCESS);
		}
		tmp = tmp->next;
	}
	//*env_ll = tmp;
	tmp = NULL;
	head = NULL;
	return (SUCCESS);
}
