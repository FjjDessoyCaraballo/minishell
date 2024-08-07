/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/06 22:57:25 by walnaimi         ###   ########.fr       */
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
    int i;
    int j;
    int len = ft_strlen(str);
    char *new_str = NULL;
    char *temp_str = NULL;

    new_str = (char *)malloc(len + 1); // Max possible length, Allocate space for the result
    if (!new_str)
        return NULL;

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            char quote_char = str[i];// Skip the opening quote
            i++;
            while (str[i] && str[i] != quote_char)
                new_str[j++] = str[i++];// Append characters between quotes
            if (str[i] == quote_char)
                i++;// Skip the closing quote
        }
        else
            new_str[j++] = str[i++];// Copy characters outside of quotes
    }
    new_str[j] = '\0'; // Null-terminate the new string
    temp_str = (char *)malloc(j + 1);// Allocate and copy the final cleaned string
    if (!temp_str)
    {
        free(new_str);
        return NULL;
    }
    ft_memcpy(temp_str, new_str, j + 1);
    free(new_str); // Free the old string

    return temp_str;
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target;
    char *token;
    char *new_token;
    int index = 0;
    int token_start = 0; // Start index of the token
    data->quote = 0;
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
                //printf("closing quote\n");//debug
                data->in_quotes = 0;
                //data->quote = data->in_quotes;
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
                data->quote = data->in_quotes;
                cur_tok->expand = false;
                //printf("curtok:%s\nexpand:%d\n\n",cur_tok->value, cur_tok->expand);
                //printf("single quotes\n");
            }
            else
            {
                data->in_quotes = 2;
                data->quote = data->in_quotes;
                cur_tok->expand = true;
                //printf("double quotes\n");
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
        data->status = 4;
        printf("Error: unmatched quote found.\n");
        return NULL;
    }

    if (index == 0 && !data->in_quotes) // No token found
        return NULL;

    // Allocate token and copy substring, excluding the opening and closing quotes if present
    token = ft_substr(target, token_start, index - token_start);
    if (!token)
        return NULL;

    // Expand environment variables in the token
    //printf("before expand:%s\n", token);
    new_token = expand_env_variables(token, data);
    //printf("after expand:%s\n", new_token);
    
    if (new_token)
    {
        free(token); // Free the old token with quotes
        token = new_token; // Update token to the expanded version
    }
    else
    {
        // Handle failure to expand
        free(token);
        token = NULL;
    }
    char *stripped_token = remove_quotes(token);
    token = stripped_token;
    // Move target pointer past the token
    target += index;

    // Skip trailing delimiters for next call
    while (*target && ft_charinstr(*target, delim))
    {
        target++;
    }
    return token;
}
