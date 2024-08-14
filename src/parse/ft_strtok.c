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

char *remove_quotes(const char *str, t_data *data)
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
            data->quote_char = str[i];// Skip the opening quote
            i++;
            while (str[i] && str[i] != data->quote_char)
                new_str[j++] = str[i++];// Append characters between quotes
            if (str[i] == data->quote_char)
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

void initialize_tokenization(t_data *data) {
    data->sindex = 0;
    data->token_start = 0; // Start data->sindex of the token
    data->quote = 0;
    data->in_quotes = 0;
    data->quote_char = '\0';
}

char *set_target_and_skip_delimiters(const char *str, const char *delim, char **target)
{
    if (str)
        *target = (char *)str;  // Cast away const for internal usage; ensure this cast is safe
    if (!*target || **target == '\0')
        return NULL;

    while (**target && ft_charinstr(**target, delim))  // Skip leading delimiters
    {
        (*target)++;
        if (**target == '\0')
            return NULL;
    }
    return *target;  // Return the updated target pointer
}

void handle_quote(const char *target, t_data *data, t_token *cur_tok)
{
    // Check the type of quote and update data accordingly
    if (target[data->sindex] == '\'')
    {
        data->in_quotes = 1;         // Single quote state
        data->quote = data->in_quotes;
        cur_tok->expand = false;     // Single quotes typically don't expand
    }
    else
    {
        data->in_quotes = 2;         // Double quote state
        data->quote = data->in_quotes;
        cur_tok->expand = true;      // Double quotes typically expand variables
    }
    
    data->quote_char = target[data->sindex]; // Record the quote character
    data->sindex++;                       // Skip the opening quote
}


void process_quoting_and_delimiters(const char *target, const char *delim, t_data *data, t_token *cur_tok)
{
    while (target[data->sindex])
    {
        if (data->in_quotes)
        {
            if (target[data->sindex] == data->quote_char)
            {
                data->in_quotes = 0;        // End quote state
                data->quote_char = '\0';   // Clear quote character
                data->sindex++;            // Move past the closing quote
                continue;
            }
        }
        else if (target[data->sindex] == '"' || target[data->sindex] == '\'')
        {
            handle_quote(target, data, cur_tok);
            continue;
        }
        else if (ft_charinstr(target[data->sindex], delim) && !data->in_quotes)
            break;
        data->sindex++;  // Move to the next character
    }
}

char *validate_and_process_token(const char *target, t_data *data)
{
    if (data->in_quotes)// Check for unmatched quotes
    {
        data->status = 4;
        return NULL;
    }
    if (data->sindex == 0 && !data->in_quotes)// Check if no token was found
        return NULL;
    data->ctoken = ft_substr(target, data->token_start, data->sindex - data->token_start);// Allocate token and copy substring, excluding the opening and closing quotes if present
    if (!data->ctoken)
        return NULL;
    data->cnew_token = expand_env_variables(data->ctoken, data);// Expand environment variables in the token
    if (data->cnew_token)
    {
        free(data->ctoken); // Free the old token with quotes
        data->ctoken = data->cnew_token; // Update token to the expanded version
    }
    else
    {
        free(data->ctoken); // Handle failure to expand
        data->ctoken = NULL;
    }
    return data->ctoken;
}

int unmatched_quote_check(t_data *data)
{
    //printf("data->echoed:%d\n",data->echoed);
    if (data->in_quotes)
    {
        if(data->echoed == 0)
            printf("unmatched quote\n");
        else
            printf("unmatched quote");
        return FAILURE;
    }
    return SUCCESS;
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target;
    initialize_tokenization(data);
   if(set_target_and_skip_delimiters(str, delim, &target) == NULL)
        return NULL;

    data->token_start = data->sindex;// Token starts here
    process_quoting_and_delimiters(target, delim, data, cur_tok);
    if (unmatched_quote_check(data) == FAILURE)
    {
        data->status = 963;
        return NULL;
    }
    char *token;
    token = validate_and_process_token(target, data);
    if (!token)
        return NULL;
    char *stripped_token;
    stripped_token = remove_quotes(data->ctoken, data);// Remove quotes from the token
    data->ctoken = stripped_token;
    target += data->sindex;// Move target pointer past the token

    while (*target && ft_charinstr(*target, delim))    // Skip trailing delimiters for next call
        target++;
    return data->ctoken;
}
