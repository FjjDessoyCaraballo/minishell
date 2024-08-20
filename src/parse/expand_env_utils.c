/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:53:29 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/20 15:07:41 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
void copy_env_value(char *result, const char *env_value, size_t *j, t_data *data)
{
    (void) data;
    if (env_value)
    {
        size_t value_len;
        value_len = ft_strlen(env_value);
        strncpy(&result[*j], env_value, value_len);//FT_STRNCPY THAT BICH
        *j += value_len;
    }
}

int is_key_in_list(t_env *head, const char *word)
{
    while (head != NULL)
    {
        if (head->key != NULL && strcmp(head->key, word) == 0)
        {
            return 1; // Key found
        }
        head = head->next;
    }
    return 0; // Key not found
}

// Function to count how many words in the input match keys in the list
int count_matching_keys(t_env *env_list, const char *input)
{
    int count = 0;
    int index = 0;
    char *word = NULL;
    
    const char *ptr = input;
    
    while (*ptr != '\0')
    {
        while (*ptr != '\0' && isspace(*ptr))
            ptr++;
        if (*ptr == '\'' || *ptr == '"')
            ptr++;
        if (*ptr == '$')
            ptr++;
        index = 0;
        while (*ptr != '\0' && !isspace(*ptr) && *ptr != '\'' && *ptr != '"')
        {
            word = (char *)realloc(word, index + 2); // Dynamic reallocation
            word[index++] = *ptr++;
        }
        word[index] = '\0';
        // If a word was found, check if it is in the env list
        if (index > 0 && is_key_in_list(env_list, word))
            count++;
    }
    free(word);
    return count;
}

void setup_env_variables(const char *input,t_data *data)
{
    (void) input;
    //int i = count_matching_keys(data->envll, input);
    data->s_quote_o = 0;
    data->d_quote_o = 0;
}

void single_q(const char *input, t_data *data, char *res, size_t *i, size_t *j)
{
    data->s_quote_o = !data->s_quote_o;
    res[(*j)++] = input[(*i)++];
}

void double_q(const char *input, t_data *data, char *res, size_t *i, size_t *j)
{
    data->d_quote_o = !data->d_quote_o;
    res[(*j)++] = input[(*i)++];
}

void handle_env_variable(const char *input, size_t *i, t_data *data, char *result, size_t *j)
{
    size_t new_len;
    char *env_value = expand_env_variable(input, i, data, &new_len);
    if (env_value)
        copy_env_value(result, env_value, j, data);
}
