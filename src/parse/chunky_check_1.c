
#include "../../includes/minishell.h"

/**
 * Checks if the given token is a built-in command.
 * 
 * @param token The token to check.
 * @param current_token The current token being processed.
 * @param data The data structure containing information about the shell state.
 * 
 * @return 0 if the token is a built-in command, 1 otherwise.
 */
int check_builtin(char *token, t_token *current_token, t_data *data)
{
    if (data->echoed == false && ft_builtin_check(token, current_token) == 0)
    {
        data->cmd_ignore = true;
        if (ft_strcmp(current_token->value, "echo") == 0)
        {
            data->echoed = true;
            data->echo_flag = true;
        }
        return 0;
    }
    return 1;
}

int check_echo_flag(char *token, t_token *current_token, t_data *data) 
{
    if (data->echo_flag && token[0] == '-')
    {
        int i = 1;
        while (token[i] == 'n')
            i++;
        if (token[i] == '\0') // Ensures the token is only composed of '-n...n'
        {
            current_token->type = FLAG;
            current_token->value = "-n";
            return 0;
        }
    }
    return 1;
}

int check_flag(char *token, t_token *current_token, t_data *data)
{
    if (current_token->prev != NULL && !data->echoed &&
        (current_token->prev->type == COMMAND || current_token->prev->type == FLAG) &&
        token[0] == '-')
    {
        current_token->type = FLAG;
        current_token->value = ft_strdup(token);
        return 0;
    }
    return 1;
}