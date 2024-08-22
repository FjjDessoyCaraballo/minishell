/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 01:50:05 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



void initialize_tokenization(t_data *data)
{
    data->sindex = 0;
    data->token_start = 0; // Start data->sindex of the token
    data->quote = 0;
    data->in_quotes = 0;
    data->quote_char = '\0';
}

char *skip_starting_delim(const char *str, const char *delim, char **target)
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

void remove_quotes_and_skip_delimiters(const char *delim, t_data *data, char **target)
{
    data->ctoken = remove_quotes(data->ctoken, data); // Remove quotes from the token
    *target += data->sindex; // Move target pointer past the token
    if(data->fin_tok != NULL)
    {
        free(data->fin_tok);
        data->fin_tok = NULL;
    }
    data->fin_tok = ft_strdup(data->ctoken);
    free(data->ctoken);
    data->ctoken = NULL;
    while (**target && ft_charinstr(**target, delim)) // Skip trailing delimiters for next call
        (*target)++;
}

void process_quoting_and_delimiters(const char *target, const char *delim, t_data *data, t_token *cur_tok)
{
    data->sindex = 0;
    while (target[data->sindex])
    {
        if (data->in_quotes)
        {
            if (target[data->sindex] == data->quote_char)
            {
                data->in_quotes = 0;       // End quote state
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

int unmatched_quote_check(t_data *data)
{
    //printf("data->echoed:%d\n",data->echoed);
    if (data->in_quotes)
    {
        printf("syntax error: unmatched quote 😳\n");
        return (1);
    }
    return (0);
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target = NULL;
    char *token = NULL;
    if (str)
    {
        initialize_tokenization(data);
        data->new_str = modify_str(str);  // Allocate and modify string
        target = data->new_str;          // Set target to the modified string
    }
    if (!target || skip_starting_delim(target, delim, &target) == NULL)
    {
        //free(data->new_str);  // Free the modified string if it's no longer needed
        return NULL;
    }
    process_quoting_and_delimiters(target, delim, data, cur_tok);
    if (unmatched_quote_check(data) == 1)
    {
        data->status = 963;
        //free(data->new_str);  // Free the modified string in case of error
        return NULL;
    }
    token = substr_and_expand(target, data);  // Get the next token
    if (!token)
    {
        //free(data->new_str);  // Free the modified string if token is NULL
        return NULL;
    }
    // Set in_quotes based on the token
    if (token[0] == '\'' || token[0] == '\"')
        cur_tok->in_quotes = true;
    else
        cur_tok->in_quotes = false;
    remove_quotes_and_skip_delimiters(delim, data, &target);
    free(token);  // Free the token once done
    // Return the current token from data
    return data->fin_tok;
}
