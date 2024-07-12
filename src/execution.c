/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/12 14:29:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;
*/
int	execution(t_data *data, t_env **env_ll)
{
	t_token *token = data->token;

	token->value = data->line_read;
	if (token->type == BUILTIN)
		built_ins(data, token, env_ll);
	// else if (token->type == COMMAND)
	// 	lonely_execution(data, token, env_ll);
	return (SUCCESS);
}




