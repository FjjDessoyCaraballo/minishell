/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:11:11 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/22 16:34:20 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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

/**
 * Handles a quote character in the target string,
 * updating the data structure accordingly.
 * It checks if the current character is a single quote and sets the
 * in_quotes flag and quote_char variable in the data structure.
 * The function also increments the index in the data structure.
 *
 * @param target The string being processed.
 * @param data The data structure containing the current index and quote state.
 *
 * @return None
 */
void handle_quote(const char *target, t_data *data)
{
    if (target[data->i] == '\'')
        data->in_quotes = 1;
    else
        data->in_quotes = 2;
    data->quote_char = target[data->i];
    data->i++;
}

/**
 * Extracts a substring from the target string based on the data structure's
 * current index and token start position, and then expands any environment
 * variables within the substring.
 *
 * @param target The string from which to extract the substring.
 * @param data   The data structure containing the current index and token start
 *               position, as well as the environment variables to expand.
 *
 * @return The expanded substring, or NULL if an error occurs or no token is found.
 */
char *substr_and_expand(const char *target, t_data *data)
{
    if (data->in_quotes)// Check for unmatched quotes
    {
        data->status = 963;
        return (NULL);
    }
    if (data->i == 0 && !data->in_quotes)// Check if no token was found
        return (NULL);
    data->ctoken = ft_substr(target, data->tok_srt, data->i - data->tok_srt);

    if (!data->ctoken)
        return (NULL);
    data->cnew_token = expand_env_variables(data->ctoken, data);
    if (data->cnew_token)
    {
        free(data->ctoken);
        data->ctoken = data->cnew_token;
    }
    else
    {
        free(data->ctoken);
        data->ctoken = NULL;
    }
    return (data->ctoken);
}

// int handle_special_chars(const char *tgt, const char *s_chars, t_data *data)
// {
//     // Check for multi-character special tokens first
//     if (strncmp(&tgt[data->i], ">>", 2) == 0 
//         || strncmp(&tgt[data->i], "<<", 2) == 0)
//     {
//         data->i += 2;
//         return (2);
//     }
//     // Check for single-character special tokens
//     if (ft_charinstr(tgt[data->i], s_chars))
//     {
//         data->i++;
//         return (1);
//     }
//     return (0); // No special character found
// }

/**
 * Handles a quoted segment within a string, extracting the characters between quotes.
 * 
 * @param str The original string containing the quoted segment.
 * @param i A pointer to the current index within the string.
 * @param j A pointer to the current index within the new string.
 * @param new_str The new string to store the extracted characters.
 * @param data A pointer to the data structure containing relevant information.
 */
void handle_quoted_segment(const char *str, int *i, int *j, char *new_str, t_data *data)
{
    data->quote_char = str[*i]; // Use a local variable for the quote character
    (*i)++; // Skip the opening quote
    while (str[*i] && str[*i] != data->quote_char)
        new_str[(*j)++] = str[(*i)++]; // Append characters between quotes
    if (str[*i] == data->quote_char)
        (*i)++; // Skip the closing quote
}

/**
 * Removes quotes from a string and returns a new string without the quotes.
 *
 * @param str The input string to remove quotes from.
 * @param data Pointer to a t_data struct used for handling quoted segments.
 * @return A new string without the quotes, or NULL if memory allocation fails.
 */
char *remove_quotes(const char *str, t_data *data)
{
    int i;
    int j;
    i = 0;
    j = 0;
    char *new_str = (char *)malloc(ft_strlen(str) + 1);
    if (!new_str)
        return NULL;

    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
            handle_quoted_segment(str, &i, &j, new_str, data);
        else
            new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    return new_str;
}

