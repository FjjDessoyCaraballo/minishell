#include "../../includes/minishell.h"

char	*ft_getenv(const char *key, t_env *env_ll)
{
	t_env *env = env_ll;
	size_t key_len = ft_strlen(key);

	while(env)
	{
		if(ft_strncmp(env->content, key, key_len) == SUCCESS && env->content[key_len] == '=')
			return env->content + key_len + 1;
		env = env->next;
	}
	return NULL;
}