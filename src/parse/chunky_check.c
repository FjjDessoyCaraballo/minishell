/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/23 04:34:51 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_check(char *token, t_token *current_token)
{
	if (ft_strncmp(token, "echo", 5) == 0
		|| ft_strncmp(token, "exit", 5) == 0
		|| ft_strncmp(token, "pwd", 4) == 0
		|| ft_strncmp(token, "cd", 3) == 0
		|| ft_strncmp(token, "export", 7) == 0
		|| ft_strncmp(token, "unset", 6) == 0
		|| ft_strncmp(token, "env", 3) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = BUILTIN;
		free(token);
		token = NULL;
		return (0);
	}
	return (1);
}

int	cmd_check(char *token, t_token *current_token, t_data *data)
{
	if (handle_absolute_path(token, current_token) == 0)
		return (0);
	if (handle_cmd_exe(token, current_token, data) == 0)
		return (0);
	return (1);
}

int	ft_pipe_check(char *token, t_token *current_token)
{
	if (ft_strcmp(token, "|") == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = PIPE;
		free(token);
		token = NULL;
		return (0);
	}
	return (1);
}

int	ft_redirect_op_check(char *token, t_token *current_token)
{
	if (handle_redirect_out(token, current_token) == 0)
		return (0);
	if (handle_append(token, current_token) == 0)
		return (0);
	if (handle_redirect_in(token, current_token) == 0)
		return (0);
	if (handle_heredoc(token, current_token) == 0)
		return (0);
	return (1);
}

int	ft_argument_check(char *token, t_token *current_token)
{
	current_token->value = ft_strdup(token);
	current_token->type = ARG;
	free(token);
	token = NULL;
	return (0);
}
