/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:00 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 13:16:15 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void initialize_tokenization(t_data *data)
{
    data->sindex = 0;
    data->token_start = 0;
    data->quote = 0;
    data->in_quotes = 0;
    data->quote_char = '\0';
}

char *skip_starting_delim(const char *str, const char *delim, char **target)
{
    if (str)
        *target = (char *)str;
    if (!*target || **target == '\0')
        return NULL;
    while (**target && ft_charinstr(**target, delim))
    {
        (*target)++;
        if (**target == '\0')
            return NULL;
    }
    return *target;
}

void remove_quotes_and_skip_delimiters(const char *delim, t_data *data, char **target)
{
    data->ctoken = remove_quotes(data->ctoken, data);
    *target += data->sindex;
    if (data->fin_tok != NULL)
        free_null(data->fin_tok);
    data->fin_tok = ft_strdup(data->ctoken);
    free_null(data->ctoken);
    while (**target && ft_charinstr(**target, delim))
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
                data->in_quotes = 0;
                data->quote_char = '\0';
                data->sindex++;           
                continue ;
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
    if (data->in_quotes)
    {
        printf("syntax error: unmatched quote 😳\n");
        return (1);
    }
    return (0);
}

char *ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok)
{
    static char *target;
    char *token;

    token = NULL;
    if (str)
    {
        initialize_tokenization(data);
        data->new_str = modify_str(str);
        target = data->new_str;
    }
    if (!target || skip_starting_delim(target, delim, &target) == NULL)
    {
        //free(data->new_str);
        return NULL;
    }
    process_quoting_and_delimiters(target, delim, data, cur_tok);
    if (unmatched_quote_check(data) == 1)
    {
        data->status = 963;
        //free(data->new_str);  // Free the modified string in case of error
        return NULL;
    }
    token = substr_and_expand(target, data);
    if (!token)
    {
        free(data->new_str);
        return NULL;
    }
    if (token[0] == '\'' || token[0] == '\"')
        cur_tok->in_quotes = true;
    else
        cur_tok->in_quotes = false;
    remove_quotes_and_skip_delimiters(delim, data, &target);
    free(token);
    return  (data->fin_tok);
}
