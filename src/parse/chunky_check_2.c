#include "../../includes/minishell.h"

int check_pipe(char *token, t_token *current_token, t_data *data)
{
    if (ft_pipe_check(token, current_token) == 0)
    {
        data->cmd_ignore = false;
        current_token->echo = false;
        data->echoed = false;
        return 0;
    }
    return 1;
}

int check_redirect(char *token, t_token *current_token, t_data *data)
{
    if (ft_redirect_op_check(token, current_token) == 0)
    {
        current_token->echo = false;
        data->echoed = false;
        return 0;
    }
    return 1;
}

int check_command(char *token, t_token *current_token, t_data *data)
{
    if (!data->echoed && !data->cmd_ignore && (ft_command_check(token, current_token, data) == 0))
    {
        data->cmd_ignore = true;
        return 0;
    }
    return 1;
}

int check_argument(char *token, t_token *current_token, t_data *data)
{
    if (ft_argument_check(token, current_token) == 0)
    {
        if (current_token->id == 1 && current_token->prev->type == ARG)
            data->cmd_ignore = true;
        if (data->echoed == true)
            current_token->echo = true;
        return 0;
    }
    return 1;
}