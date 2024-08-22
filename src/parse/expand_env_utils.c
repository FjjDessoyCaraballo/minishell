/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:53:29 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/22 05:08:56 by walnaimi         ###   ########.fr       */
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
    int max_word_size = ft_strlen(input); // Get the length of the input string

    // Allocate memory for the word buffer, with a little extra space for safety
    char *word = (char *)malloc((max_word_size + 1) * sizeof(char)); 

    if (!word) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1; // Return an error code if malloc fails
    }
    const char *ptr = input;

    while (*ptr != '\0')
    {
        while (*ptr != '\0' && isspace(*ptr))// Skip leading spaces
            ptr++;
        if (*ptr == '\'' || *ptr == '"')// Skip quotes
            ptr++;
        if (*ptr == '$')// Skip dollar sign
            ptr++;
        index = 0;// Reset index for the new word
        while (*ptr != '\0' && !isspace(*ptr) && *ptr != '\'' && *ptr != '"')// Extract the word
        {
            if (index < max_word_size) // Ensure we stay within bounds
                word[index++] = *ptr++;
            else
                break;
        }
        word[index] = '\0';// Null-terminate the word
        if (index > 0 && is_key_in_list(env_list, word))// Check if the word is in the environment list
            count++;
        while (*ptr != '\0' && (isspace(*ptr) || *ptr == '\'' || *ptr == '"'))// Advance pointer to skip any trailing quote or space after the word
            ptr++;
    }
    free(word); // Free the dynamically allocated memory
    return count;
}



void setup_env_variables(const char *input,t_data *data)
{
    data->num_of_envs = count_matching_keys(data->envll, input);
    if (data->num_of_envs == 0)
        data->num_of_envs = 1;
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
