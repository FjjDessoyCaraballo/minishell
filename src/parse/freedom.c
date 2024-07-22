#include "../../includes/minishell.h"

void free_my_boi(char **paths)
{
	int i;
	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
