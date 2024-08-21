/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/21 14:25:46 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_gang(t_data *data)
{
	if(data->tok_res != NULL)
	{
		free(data->tok_res);
		data->tok_res = NULL;
	}
	if(data->ctoken != NULL)
	{
		free(data->ctoken);
		data->ctoken = NULL;
	}
	if(data->temp_str != NULL)
	{
		free(data->temp_str);
		data->temp_str = NULL;
	}
	if(data->new_str != NULL)
	{
		free(data->new_str);
		data->new_str = NULL;
	}
	free_tokens(data->token);
}

int	main(int argc, char **argv, char **env)
{
	t_data			*data;
	t_env			**env_ll;
	static int		status;
	
	(void)argv;
	data = ft_calloc(1, sizeof(t_data));
	env_ll = ft_calloc(1, sizeof(t_env *));
	if (!data  || !env_ll)
		return (1);
	if (argc == 1)
	{
		initializer(data, env_ll, env);
		while (666)
		{
			status = sniff_line(data);
			if (status == NULL_LINE)
			{
				printf("exit\n");
				break ;
			}
			else if (status == 963)
				continue ;
			else
				execution(data, env_ll);
			free_gang(data);
		}
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	free_array(data->binary_paths);
	free_ll((*env_ll));
	free_null(env_ll);
	free(data);
	return (0);
}
