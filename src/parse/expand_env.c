#include "../../includes/minishell.h"

char *expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len)
{
    size_t var_start = ++(*i);
    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
        (*i)++; // Find end of variable name

    size_t var_len = *i - var_start;
    if (var_len > 0) {
        char var_name[var_len + 1];
        strncpy(var_name, &input[var_start], var_len);
        var_name[var_len] = '\0';

        char *env_value = ft_getenv(var_name, data->envll);
        if (env_value) {
            *new_len = strlen(env_value);
            return env_value;
        } else {
            return NULL;
        }
    }
    return NULL;
}

void copy_env_value(char *result, const char *env_value, size_t *j, const char *var_name)
{
    if (env_value) {
        size_t value_len = strlen(env_value);
        strncpy(&result[*j], env_value, value_len);
        *j += value_len;
    } else {
        result[(*j)++] = '$';
        strncpy(&result[*j], var_name, strlen(var_name));
        *j += strlen(var_name);
    }
}

char *expand_env_variables(const char *input, t_data *data)
{
    size_t len = strlen(input);
    char *result = (char *)malloc(len * 2);
    if (!result) return NULL;

    size_t i = 0, j = 0;
    while (input[i]) {
        if (input[i] == '$') {
            size_t new_len;
            char *env_value = expand_env_variable(input, &i, data, &new_len);
            if (env_value) {
                copy_env_value(result, env_value, &j, env_value);
            } else {
                result[j++] = '$';
            }
        } else {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
}