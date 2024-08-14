#include "../../includes/minishell.h"

char *get_env_value(const char *input, size_t var_start, size_t var_len, t_data *data)
{
    char var_name[var_len + 1];
    strncpy(var_name, &input[var_start], var_len);
    var_name[var_len] = '\0';

    return ft_getenv(var_name, data->envll);
}


char *expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len)
{
    size_t var_start = ++(*i);
    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
        (*i)++; // Find end of variable name

    size_t var_len = *i - var_start;
    if (var_len > 0)
    {
        /*char var_name[var_len + 1];
        strncpy(var_name, &input[var_start], var_len);
        var_name[var_len] = '\0';

        char *env_value = ft_getenv(var_name, data->envll);*/
        char *env_value = get_env_value(input, var_start, var_len, data);
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

char *expand_env_variables(const char *input, t_data *data)
{
    setup_env_variables(input, data);
    char *result;
    size_t i;
    size_t j;
    result = (char *)malloc(data->len_t * 2 + 1); // Allocate enough space for the expanded result
    if (!result)
        return NULL;
    i = 0;
    j = 0;
    while (input[i])
    {
        if (input[i] == '\'' && !data->d_quote_o)
            single_q(input, data, result, &i, &j);
        else if (input[i] == '"' && !data->s_quote_o)
            double_q(input, data, result, &i, &j);
        else if (input[i] == '$' && !data->s_quote_o)
            handle_env_variable(input, &i, data, result, &j);
        else
            result[j++] = input[i++];
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}
