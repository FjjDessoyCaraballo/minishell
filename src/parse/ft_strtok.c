/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/26 13:25:05 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int i;
	i = 0;
	
	while (s1[i] == s2[i] && s1[i])
		i++;
	return (s1[i] - s2[i]);
}
int ft_charinstr(char c, const char *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}
 
char *ft_strtok(char *str, const char *delim)
{
    static char *target;
    char *token;
    int index = 0;
    int in_quotes = 0;
    char quote_char = '\0';

    if (str)
        target = str;
    if (!target || *target == '\0')
        return NULL;

    // Skip leading delimiters
    while (*target && ft_charinstr(*target, delim))
    {
        target++;
        if (*target == '\0')
            return NULL;
    }
    
    // Token starts here
    index = 0;
    while (target[index])
    {
        if (in_quotes)
        {
            if (target[index] == quote_char)
            {
                in_quotes = 0;
                quote_char = '\0';
                // Remove quote by skipping it
                ft_memmove(&target[index], &target[index + 1], strlen(&target[index + 1]) + 1);
                index--;
            }
        }
        else if (target[index] == '"' || target[index] == '\'')
        {
            in_quotes = 1;
            quote_char = target[index];
            // Set expand to true if double quotes, false if single quotes
            //*expand = (quote_char == '"');
            // Remove quote by skipping it
            ft_memmove(&target[index], &target[index + 1], strlen(&target[index + 1]) + 1);
            index--;
        }
        else if (ft_charinstr(target[index], delim) && !in_quotes)
        {
            break;
        }
        index++;
    }

    token = ft_substr(target, 0, index);
    target += index;

    return token;
}

