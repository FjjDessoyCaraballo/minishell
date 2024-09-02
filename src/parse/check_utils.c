/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:52:13 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/02 10:22:02 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*loop_path_for_binary(char *binary, char **paths)
{
	char	*token_with_path;
	int		i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths && paths[i])
	{
		token_with_path = ft_strsjoin(paths[i], binary, '/');
		if (!access(token_with_path, F_OK))
		{
			if (!access(token_with_path, X_OK))
				return (token_with_path);
			else
			{
				err_msg(binary, NO_PERMISSION, 1);
				free_null(token_with_path);
			}
		}
		free_null(token_with_path);
		i++;
	}
	return (NULL);
}

char	*ft_strndup(const char *s, size_t n)
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	count_token(t_token *token, t_type type)
{
	t_token	*head;
	int		count;

	count = 0;
	head = token;
	while (head->next != NULL)
	{
		if (head->type == type)
			count++;
		head = head->next;
	}
	head = NULL;
	return (count);
}
