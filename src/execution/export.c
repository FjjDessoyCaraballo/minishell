/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy <fdessoy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:01:34 by fdessoy           #+#    #+#             */
/*   Updated: 2024/08/27 21:07:34 by fdessoy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* export puts variables declared by user in the env */
int	export(t_token *token, t_env **env_ll)
{
	if (should_skip_token(token))
		return (SUCCESS);
	if (token->next == NULL || token->next->value == NULL)
	{
		print_export(env_ll);
		return (SUCCESS);
	}
	token = token->next;
	process_tokens(token, env_ll);
	return (SUCCESS);
}

int	should_skip_token(t_token *token)
{
	if (find_token(token, APPEND) || find_token(token, HEREDOC)
		|| find_token(token, RED_IN) || find_token(token, RED_OUT))
		return (1);
	return (0);
}

void	process_tokens(t_token *token, t_env **env_ll)
{
	t_token	*tmp_tok;
	int		found;

	tmp_tok = token;
	while (tmp_tok != NULL)
	{
		found = process_token(tmp_tok, env_ll);
		if (!found)
			ft_listadd_back(env_ll, ft_listnew(tmp_tok->value));
		tmp_tok = tmp_tok->next;
	}
}

int	process_token(t_token *tmp_tok, t_env **env_ll)
{
	t_env	*tmp_ll;
	char	**array;

	if (tmp_tok->value == NULL || !ft_strchr(tmp_tok->value, '='))
		return (1);
	tmp_ll = (*env_ll);
	while (tmp_ll != NULL)
	{
		array = ft_split(tmp_tok->value, '=');
		if (!array)
			return (FAILURE);
		if (!ft_strncmp(tmp_ll->key, array[0], ft_strlen(tmp_ll->key)))
		{
			update_env_variable(tmp_ll, tmp_tok, array);
			free_array(array);
			return (1);
		}
		free_array(array);
		tmp_ll = tmp_ll->next;
	}
	return (0);
}

void	update_env_variable(t_env *tmp_ll, t_token *tmp_tok, char **array)
{
	if (!tmp_ll || !array || !tmp_tok->value)
		return ;
	free_null(tmp_ll->value);
	free_null(tmp_ll->key);
	free_null(tmp_ll->content);
	tmp_ll->content = ft_strdup(tmp_tok->value);
	tmp_ll->key = ft_strdup(array[0]);
	if (!tmp_ll->key)
		return ;
	tmp_ll->value = ft_strdup(ft_strchr(tmp_tok->value, '=') + 1);
	if (!tmp_ll->value)
	{
		free_null(tmp_ll->key);
		return ;
	}
}
