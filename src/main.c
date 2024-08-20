/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:12:51 by fdessoy-          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/20 10:52:48 by fdessoy-         ###   ########.fr       */
=======
/*   Updated: 2024/08/20 06:05:54 by walnaimi         ###   ########.fr       */
>>>>>>> 01d4a45 (fixed tokenization order)
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_gang(t_data *data)
{
	if(data->vtoken != NULL)
	{
		free(data->vtoken);
		data->vtoken = NULL;
	}
	if(data->ctoken != NULL)
	{
		free(data->ctoken);
		data->ctoken = NULL;
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
		// env_ll = NULL;
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
		}
		free_gang(data);
		free(data->line_read);
	}
	else
		ft_putstr_fd(ERR_ARG, 2);
	free_array(data->binary_paths);
	free_ll((*env_ll));
	free_null(env_ll);
	free(data);
	return (0);
}
