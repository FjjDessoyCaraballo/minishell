#include "../../includes/minishell.h"


void	ft_builtin_check(char* token)
{
	if (strcmp(token,"echo") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token,"cd") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token,"pwd") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token,"export") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token,"unset") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token, "env") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token, "exit") == 0)
		printf("token:%s\n", token);
	else if (strcmp(token, "\"") == 0)
		printf("token:%s\n", token);
	else
		printf("Unknown command :(%s)\n", token);
}

int		ft_charinstr(char c,const char *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

char *ft_strtok(char *str, const char *delim)
{
    static char *target;
    char *token;
    int index = 0;

    if (str)
        target = str; 
    if (*target == '\0')
        return NULL;

    // Skip leading delimiters
    while (*target && ft_charinstr(*target, delim))
        target++;
    // Determine token boundaries
    index = 0;
    while (target[index] && (!ft_charinstr(target[index], delim)))
		index++;
    token = ft_substr(target, 0, index);
    target += index;
    return token;
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
