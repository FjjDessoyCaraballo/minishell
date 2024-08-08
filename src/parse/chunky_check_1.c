
#include "../../includes/minishell.h"

int check_builtin(char *token, t_token *current_token, t_data *data)
{
    if (ft_builtin_check(token, current_token) == SUCCESS)
    {
        data->cmd_ignore = true;
        if (ft_strcmp(current_token->value, "echo") == SUCCESS)
        {
            data->echoed = true;
            data->echo_flag = true;
        }
        return SUCCESS;
    }
    return FAILURE;
}

int check_echo_flag(char *token, t_token *current_token, t_data *data) 
{
    (void)token;
    if (data->echo_flag && ft_strncmp(current_token->value, "-n", 3) == SUCCESS)
    {
        current_token->type = FLAG;
        current_token->value = "-n";
        return SUCCESS;
    }
    return FAILURE;
}

int check_flag(char *token, t_token *current_token, t_data *data)
{
    if (current_token->prev != NULL && !data->echoed &&
        (current_token->prev->type == COMMAND || current_token->prev->type == FLAG) &&
        token[0] == '-')
    {
        current_token->type = FLAG;
        current_token->value = ft_strdup(token);
        return SUCCESS;
    }
    return FAILURE;
}