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

void handle_status_variable(t_data *data, char *result, size_t *j) 
{
    char *status_str = ft_itoa(data->status); // Convert integer to string
    if (status_str)
    {
        size_t k = 0;
        while (status_str[k])
            result[(*j)++] = status_str[k++]; // Append each character to result
        free(status_str); // Free the dynamically allocated memory
    }
}

void handle_dollar_sign(const char *input, size_t *i, t_data *data, char *result, size_t *j)
{
    if (input[*i + 1] == ' ' || input[*i + 1] == '\0') 
        result[(*j)++] = input[(*i)++];
    else if (data->d_quote_o && (input[*i + 1] == '"' || input[*i + 1] == '\''))
        result[(*j)++] = input[(*i)++];
    else if (input[*i + 1] == '"')
        result[(*j)++] = input[(*i)++];
    else if (input[*i + 1] == '$')
    {
        (*i) += 2;
        result[(*j)++] = '$';
    }
    else if (input[*i + 1] == '?')
    {
        (*i) += 2;
        handle_status_variable(data, result, j);
    }
    else
    {
        if(data->echoed == 1)
        //printf("data->echoed:%d\n",data->echoed);//debug
        handle_env_variable(input, i, data, result, j);
    }
}


char *expand_env_variables(const char *input, t_data *data)
{
    setup_env_variables(input, data);
    char *result;
    size_t i;
    size_t j;
    result = (char *)malloc(MAX_ARG_STR); // Allocate enough space for the expanded result
    /*size_t bruh = data->len_t * 2 + 1;
    printf("\nbruh: %lu\n", bruh);//debug*/
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
        {
            handle_dollar_sign(input, &i, data, result, &j);
            /*if(data->env_copied == 1)
            {
                //printf("\n----------\ndata->len_c: %lu\n----------\n", data->len_env);//debug
                //free(result);
                ///result =(char *) malloc(data->len_env * 2 + 1);
                data->env_copied = 0;
            }*/
        }
        else
            result[j++] = input[i++];
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}
