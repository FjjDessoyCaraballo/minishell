#include "../../includes/minishell.h"

int ft_charinstr(char c, const char *str)
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
    if (!target || *target == '\0')
        return NULL;

    while (*target && ft_charinstr(*target, delim))//skip the delimiters before
	{
        target++;
		if(*target == '\0')
			return NULL;
	}
    index = 0;
    while (target[index] && (!ft_charinstr(target[index], delim)))//once find a non-delimiter will start counting
		index++;
    token = ft_substr(target, 0, index);
    target += index;
	while (*target && ft_charinstr(*target, delim))//skip the delimiters after
		target++;
    return token;
}

