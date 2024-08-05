/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 10:05:41 by fdessoy-         ###   ########.fr       */
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

int     ft_getstart(char const *s1, char const *set)
{
        size_t  len;
        size_t  i;

        len = ft_strlen(s1);
        i = 0;
        while (i < len)
        {
                if (ft_strchr(set, s1[i]) == 0)
                        break ;
                i++;
        }
        return (i);
}

int     ft_getend(const char *s1, const char *set)
{
        size_t  len;
        size_t  i;

        len = ft_strlen(s1);
        i = 0;
        while (i < len)
        {
                if (ft_strchr(set, s1[len - i - 1]) == 0)
                        break ;
                i++;
        }
        return (len - i);
}

char    *ft_strtrim(char const *s1, char const *set)
{
        int             end;
        int             start;
        char    *dest;

        if (s1 == NULL)
                return (NULL);
        if (set == NULL)
                return (ft_strdup(s1));
        start = ft_getstart(s1, set);
        end = ft_getend(s1, set);
        if (start >= end)
                return (ft_strdup(""));
        dest = (char *)malloc(sizeof(char) * (end - start + 1));
        if (dest == NULL)
                return (NULL);
        ft_strlcpy(dest, s1 + start, end - start + 1);
        return (dest);
}

int ft_strlencount(const char *str, char c, int numtof)
{
    int i;
    i = 0;
    int found;
    found = 0;
    while(str[i] && found != numtof)
    {
        if(str[i] == c)
            found++;
        i++;
    }
    return(i);
}
char *remove_quotes(const char *str)
{
    int i, j;
    int len = strlen(str);
    //int new_len = 0;
    char *new_str = NULL;
    char *temp_str = NULL;

    // Allocate space for the result
    new_str = (char *)malloc(len + 1); // Max possible length
    if (!new_str)
        return NULL;

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            // Skip the opening quote
            char quote_char = str[i];
            i++;
            while (str[i] && str[i] != quote_char)
            {
                // Append characters between quotes
                new_str[j++] = str[i++];
            }
            if (str[i] == quote_char)
            {
                // Skip the closing quote
                i++;
            }
        }
        else
        {
            // Copy characters outside of quotes
            new_str[j++] = str[i++];
        }
    }
    new_str[j] = '\0'; // Null-terminate the new string

    // Allocate and copy the final cleaned string
    temp_str = (char *)malloc(j + 1);
    if (!temp_str)
    {
        free(new_str);
        return NULL;
    }
    memcpy(temp_str, new_str, j + 1);
    free(new_str); // Free the old string

    return temp_str;
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target;
    char *token;
    int index = 0;
    int token_start = 0; // Start index of the token
    data->error = 0;
    data->in_quotes = 0;
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
    token_start = index;
    while (target[index])
    {
        if (data->in_quotes)
        {
            if (target[index] == quote_char)
            {
                data->in_quotes = 0;
                quote_char = '\0';
                index++; // Move past the closing quote
                continue;
            }
        }
        else if (target[index] == '"' || target[index] == '\'')
        {
            if(target[index] == '\'')
            {
                data->in_quotes = 1;
                cur_tok->expand = false;
            }
            else
            {
                data->in_quotes = 2;
                cur_tok->expand = true;
            }
            quote_char = target[index];
            index++; // Skip the opening quote
            continue;
        }
        else if (ft_charinstr(target[index], delim) && !data->in_quotes)
        {
            break;
        }
        index++;
    }

    if (data->in_quotes)
    {
        data->error = 4;
        printf("Error: unmatched quote found.\n");
        return NULL;
    }

    if (index == 0 && !data->in_quotes) // No token found
        return NULL;

    // Allocate token and copy substring, excluding the opening and closing quotes if present
    token = ft_substr(target, token_start, index - token_start);
    if (!token)
        return NULL;

    // Remove quotes from the token
    //printf("before strippin:%s\n",token);
    char *stripped_token = remove_quotes(token);
    free(token); // Free the original token with quotes

    // Move target pointer past the token
    target += index;

    // Skip trailing delimiters for next call
    while (*target && ft_charinstr(*target, delim))
    {
        target++;
    }

    return stripped_token;
}
