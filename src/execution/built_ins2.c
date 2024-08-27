/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy <fdessoy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 17:12:24 by fdessoy          ###   ########.fr       */
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


// int	unset(t_token *token, t_env **env_ll)
// {
// 	t_env	*tmp;
// 	t_env	*del;
// 	t_token	*head;

// 	head = token;
// 	if (!head->next->value || !*env_ll || !env_ll)
// 		return (SUCCESS);
// 	tmp = *env_ll;
// 	head = head->next;
// 	while (!ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
// 	{
// 		tmp = tmp->next;
// 		free_null(tmp->key);
// 		free_null(tmp->value);
// 		free_null(tmp->content);
// 		free(tmp);
// 		tmp = NULL;
// 		return (SUCCESS);
// 	}
// 	tmp = *env_ll;
// 	while (tmp->next != NULL)
// 	{
// 		if (!ft_strncmp(head->value, tmp->next->content,
// 				ft_strlen(head->value)))
// 		{
// 			del = tmp->next;
// 			tmp->next = tmp->next->next;
// 			free_null(del->key);
// 			free_null(del->value);
// 			free_null(del->content);
// 			free(del);
// 			del = NULL;
// 			return (SUCCESS);
// 		}
// 		tmp = tmp->next;
// 	}
// 	//*env_ll = tmp;
// 	tmp = NULL;
// 	head = NULL;
// 	return (SUCCESS);
// }
