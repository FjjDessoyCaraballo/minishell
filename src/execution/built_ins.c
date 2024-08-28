/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/26 16:31:36 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* This part is taking the tokens and taking the argument just after it. */
int	built_ins(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	status = 0;
	if (!ft_strncmp(token->value, "env", 4))
		status = print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 4))
		status = print_pwd();
	else if (!ft_strncmp(token->value, "exit", 5))
		get_the_hell_out(data, token, env_ll);
	else if (!ft_strncmp(token->value, "echo", 5))
		status = yodeling(token);
	else if (!ft_strncmp(token->value, "cd", 3))
		status = shell_cd(token, data);
	else if (!ft_strncmp(token->value, "export", 7))
		status = export(token, env_ll);
	else if (!ft_strncmp(token->value, "unset", 6))
		status = unset(token, env_ll);
	return (status);
}

/* The printing of the environment changes in conformity to the use of
export and unset. The command 'env' itself does not take arguments.
e.g. $> env || $> pwd (no white spaces or anything like caps)*/
int	print_env(t_env *env_ll)
{
	t_env	*tmp;

	if (!env_ll)
		return (FAILURE);
	tmp = env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	env_ll = tmp;
	return (SUCCESS);
}

int	print_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (FAILURE);
	printf("%s\n", pwd);
	free_null(pwd);
	return (SUCCESS);
}

/* This is the exit function, it needs to take, if inputted,
an exit code that was manually inserted after exit */
void	get_the_hell_out(t_data *data, t_token *token, t_env **env_ll)
{
	int status;
	status = 0;
	free_all_ll(env_ll);
	ft_printf("exit\n");
	if (token->next != NULL && token->next->value != NULL)
	{
		status = ft_atoi(token->next->value);
		free_gang(data);
		exit(status);
	}
	free_gang(data);
	exit(data->status);
}

int	handle_flag_type(t_token *head)
{
	head = head->next;
	while (head->type == FLAG)
	{
		head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
	}
	while (head->value != NULL)
	{
		while (head->value != NULL && head->type == ARG
			&& head->value[0] == '\0')
			head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
		if (head->type == RED_IN || head->type == RED_OUT
			|| head->type == APPEND || head->type == HEREDOC)
			break ;
		printf("%s", head->value);
		if (head->next->value != NULL && head->next->empty == false)
			printf(" ");
		head = head->next;
	}
	return (SUCCESS);
}

int	handle_arg_type(t_token *head)
{
	head = head->next;
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head->next != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	printf("\n");
	return (SUCCESS);
}

int	yodeling(t_token *token)
{
    t_token *head;
    head = token;
    if (head->next->value == NULL)
        return (printf("\n"), SUCCESS);
    if (head->next->type == FLAG)
        return handle_flag_type(head);
    if (head != NULL && head->next != NULL && head->next->type == ARG)
        return handle_arg_type(head);
    return (FAILURE);
}