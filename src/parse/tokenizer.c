#include "../../includes/minishell.h"

void	ft_builtin_check(char* token)
{
	if (ft_strcmp(token,"echo") == 0)
		printf("token:[%s]\n", token);
	/*else if (strcmp(token, "getenv") == 0)
	{
		char *path_env = getenv("PATH");
		if (!path_env)
			printf("");
		printf("LOOK A PATH\n\n %s", path_env);
	}*/
	else
		printf("Unknown command:[%s]\n", token);
}

void line_tokenization(t_data *data)
{
    char *token;
    const char *delimiters = "  \t\n";

    token = ft_strtok(data->line_read, delimiters);
    while (token != NULL)
    {
		ft_builtin_check(token);// this command print shit, you can comment it
		free(token);
        token = ft_strtok(NULL, delimiters);
    }
}
