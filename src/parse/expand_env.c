#include "../../includes/minishell.h"
/*
char *expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len)
{
    size_t var_start = ++(*i);
    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
        (*i)++; // Find end of variable name

    size_t var_len = *i - var_start;
    if (var_len > 0)
    {
        char var_name[var_len + 1];
        strncpy(var_name, &input[var_start], var_len);
        var_name[var_len] = '\0';

        char *env_value = ft_getenv(var_name, data->envll);
        if (env_value)
        {
            *new_len = strlen(env_value);
            return env_value;
        }
        else
        {
            *new_len = 0;
            return NULL;
        }
    }
    *new_len = 0;
    return NULL;
}

void copy_env_value(char *result, const char *env_value, size_t *j)
{
    if (env_value)
    {
        size_t value_len = strlen(env_value);
        strncpy(&result[*j], env_value, value_len);
        *j += value_len;
    }
}

char *expand_env_variables(const char *input, t_data *data)
{
    size_t len = strlen(input);
    char *result = (char *)malloc(len * 2 + 1); // Allocate enough space for the expanded result
    if (!result) return NULL;

    size_t i = 0, j = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            if (i > 0 && (input[i - 1] == '"' || input[i - 1] == '\'') && (input[i + 1] == '"' || input[i + 1] == '\''))
            {
                result[j++] = '$';
                i++;
                continue;
            }
            size_t new_len;
            char *env_value = expand_env_variable(input, &i, data, &new_len);
            if (env_value)
            {
                copy_env_value(result, env_value, &j);
            }
            // If env_value is NULL, simply skip the variable
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}
*/

char *expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len)
{
    size_t var_start = ++(*i);
    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
        (*i)++; // Find end of variable name

    size_t var_len = *i - var_start;
    if (var_len > 0)
    {
        char var_name[var_len + 1];
        strncpy(var_name, &input[var_start], var_len);
        var_name[var_len] = '\0';

        char *env_value = ft_getenv(var_name, data->envll);
        if (env_value)
        {
            *new_len = strlen(env_value);
            return env_value;
        }
        else
        {
            *new_len = 0;
            return NULL;
        }
    }
    *new_len = 0;
    return NULL;
}

void copy_env_value(char *result, const char *env_value, size_t *j)
{
    if (env_value)
    {
        size_t value_len = strlen(env_value);
        strncpy(&result[*j], env_value, value_len);
        *j += value_len;
    }
}

char *expand_env_variables(const char *input, t_data *data)
{
    size_t len = strlen(input);
    char *result = (char *)malloc(len * 2 + 1); // Allocate enough space for the expanded result
    if (!result) return NULL;

    size_t i = 0, j = 0;
    int single_quote_open = 0;
    int double_quote_open = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !double_quote_open)
        {
            single_quote_open = !single_quote_open;
            result[j++] = input[i++];
        }
        else if (input[i] == '"' && !single_quote_open)
        {
            double_quote_open = !double_quote_open;
            result[j++] = input[i++];
        }
        else if (input[i] == '$' && !single_quote_open)
        {
            size_t new_len;
            char *env_value = expand_env_variable(input, &i, data, &new_len);
            if (env_value)
            {
                copy_env_value(result, env_value, &j);
            }
            // If env_value is NULL, simply skip the variable
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}