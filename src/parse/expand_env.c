#include "../../includes/minishell.h"

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
        size_t value_len;
        value_len = ft_strlen(env_value);
        strncpy(&result[*j], env_value, value_len);
        *j += value_len;
    }
}
void    expand_init(t_data *data, const char *input)
{
    data->len_t = strlen(input);
    data->i_t = 0;
    data->j_t = 0;
    data->s_quote_o = 0;
    data->d_quote_o = 0;
}

void handle_quotes(const char *input, t_data *data, char *result)
{
    if (input[data->i_t] == '\'' && !data->d_quote_o)
    {
        data->s_quote_o = !data->s_quote_o;
        result[data->j_t++] = input[data->i_t++];
    }
    else if (input[data->i_t] == '"' && !data->s_quote_o)
    {
        data->d_quote_o = !data->d_quote_o;
        result[data->j_t++] = input[data->i_t++];
    }
}

void expand_vars_and_copy(const char *input, t_data *data, char *result)
{
    if (input[data->i_t] == '$' && !data->s_quote_o)
    {
        size_t new_len;
        char *env_value = expand_env_variable(input, &data->i_t, data, &new_len);
        if (env_value)
            copy_env_value(result, env_value, &data->j_t);
    }
    else
        result[data->j_t++] = input[data->i_t++];
}

char *expand_env_variables(const char *input, t_data *data)
{
    expand_init(data, input);
    char *result = (char *)malloc(data->len_t * 2 + 1); // Allocate enough space for the expanded result
    if (!result) return NULL;

    while (input[data->i_t])
    {
        if (input[data->i_t] == '\'' && !data->d_quote_o)
            handle_quotes(input, data, result); // Handle single and double quotes
        else if (input[data->i_t] == '$' && !data->s_quote_o)
            expand_vars_and_copy(input, data, result); // Expand environment variables
        else
            result[data->j_t++] = input[data->i_t++]; // Copy characters directly
    }
    result[data->j_t] = '\0'; // Null-terminate the result string*/
    return result;
}
