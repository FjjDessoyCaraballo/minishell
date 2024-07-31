/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:43 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/31 12:47:05 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *loop_path_for_binary(char *binary, char **paths)
{
	char	*token_with_path;
	int		i; 
	
	i = 0;
	while (paths[i])
	{
		token_with_path = ft_strsjoin(paths[i], binary, '/');
		if (!access(token_with_path, F_OK))
		{
			if (!access(token_with_path, X_OK))
				return (token_with_path);
			else
				free(token_with_path);
		}
		free(token_with_path);
		i++;
	}
	return (NULL);
}

int	check_binary_locally(char *binary, char *path)
{
	char	*binary_with_path;

	binary_with_path = ft_strsjoin(path, binary, '/');
	if (!access(binary_with_path, F_OK))
	{
		if (!access(binary_with_path, X_OK))
		{
			free(binary_with_path);
			return (SUCCESS);
		}
	}
	free(binary_with_path);
	return (FAILURE);
}

int	is_file(char *binary, char *path)
{
	char	*file_with_path;

	file_with_path = ft_strsjoin(path, binary, '/');
	if (!access(file_with_path, F_OK))
	{
		if (!access(file_with_path, X_OK))
		{
			free(file_with_path);
			return (FAILURE);
		}
		free(file_with_path);
		return (SUCCESS);
	}
	return (FAILURE);
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

int	how_many_tokens(t_token *token)
{
	t_token *head;
	int		count;

	count = 0;
	head = token;
	while (head)
	{
		if (token->type == PIPE)
			head = head->next;
		count++;
		head = head->next;	
	}
	return (count);
}
