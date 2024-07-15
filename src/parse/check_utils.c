#include "../../includes/minishell.h"

char *loop_path_for_binary(char *binary, char **paths)
{
    char *token_with_path;
    int i = 0;

    while (paths[i])
    {
        token_with_path = ft_strsjoin(paths[i], binary, '/');
        if (!access(token_with_path, F_OK))
        {
            if (!access(token_with_path, X_OK))
                return token_with_path;
            else
                free(token_with_path);
        }
        else
            free(token_with_path);
        i++;
    }
    return NULL;
}
