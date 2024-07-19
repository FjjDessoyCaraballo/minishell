/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:43 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/19 17:17:49 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *loop_path_for_binary(char *binary, char **paths)
{
    char *token_with_path;
    int i = 0;

    while (paths[i])
    {
        token_with_path = ft_strsjoin(paths[i], binary, '/');
        if (!access(token_with_path, F_OK))
        {
            if (!access(token_with_path, X_OK))
                return token_with_path;
            else
                free(token_with_path);
        }
        else
            free(token_with_path);
        i++;
    }
    return NULL;
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
