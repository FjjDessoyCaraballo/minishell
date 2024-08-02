/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:41:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/02 15:49:22 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * This function takes care of executing commands with no child_processes.
 * USAGE: %> cmd -flag argument OR %> cmd argument -flag
 */
int	single_execution(t_data *data, t_token *token, t_env **env_ll)
{
	pid_t	pid;
	int		status;

	pid = fork();
	status = 0;
	if (pid < 0)
	{
		free_ll((*env_ll));
		free_data(data, NULL, env_ll, NULL);
		perror("fork");
		data->status = -1;
		return (-1);
	}
	else if (pid == 0)
		single_child(data, token, env_ll);
	else
		status = single_parent(pid, status);
	return (status);
}

void	single_child(t_data *data, t_token *token, t_env **env_ll)
{
	char	**command_array;
	char	**env;
	char	*path;

	command_array = ttad(token, 0);
	path = ft_strsjoin(token->path, token->value, '/');
	env = env_arr_updater(env_ll);
	if (execve(path, command_array, env) == -1)
	{
		free_array(env);
		free_data(data, path, env_ll, command_array);
		exit (127);
	}
}

int	single_parent(pid_t pid, int status)
{
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (-1);
}

// /**
//  * This function rearrenges the command array in case there are redirects.
//  * The flag one is used to identify input redirect;
//  * The flag zero is used to identify output redirect;
//  */
// void	filter_redirect(t_data *data, char *instruction, int child, char *file)
// {
// 	if (!ft_strcmp(instruction, "<"))
// 		dup_fds(data, child, 1, file);
// 	else if (ft_strcmp(instruction, ">"))
// 		dup_fds(data, child, 1, file);
// }

/**
 * This is just a loop inside the parse_instruction(). Mainly done for school
 * norm reasons.
 */
char	*redirect_out(char **array, char *instruction, int flag, int index)
{
	char	*tmp;

	while (array[index])
	{
		if (ft_strcmp(array[index], ">") && flag == REDIRECT_OUT)
			break ;
		tmp = ft_strjoin(instruction, array[index]);
		if (!tmp)
			return (NULL);
		free(instruction);
		instruction = ft_strjoin(tmp, " ");
		if (!instruction)
			return (NULL);
		free(tmp);
		index++;
	}
	return (instruction);
}

/**
 * Its necessary to know which redirection we have here and give back the
 * array organized in the usual fashion of "cmd -flag" for execution. We
 * assume that the index should start at 2 because of the syntax:
 * %> "< infile cat"
 * 
 * For redirect for output, we already have at the index 0 our commands, and
 * the flags will follow as far as we hit the redirect itself. 
 * 
 * The first and second elements of the instruction, inside array_instruction,
 * MUST be the redirect and file by consequence of our parsing. At this point
 * we are working with purely validated inputs.
 * 
 * Return values: upon success, this function will return an array with only
 * the commands that will be used in execve(). In case of any failures, the
 * function returns NULL.
 */
char	**parse_instruction(char *instruction, int redirect_flag)
{
	char	**arr_instr;
	char	*parsed_cmd;
	int		index;

	arr_instr = ft_split(instruction, ' ');
	if (!arr_instr)
		return (NULL);
	parsed_cmd = ft_strdup("");
	if (!parsed_cmd)
		return (NULL);
	if (redirect_flag == REDIRECT_OUT)
		index = 0;
	else
		index = 2;
	parsed_cmd = redirect_out(arr_instr, parsed_cmd, redirect_flag, index);
	free_array(arr_instr);
	arr_instr = ft_split(parsed_cmd, ' ');
	if (!arr_instr)
	{
		free_array(arr_instr);
		return (NULL);
	}
	return (arr_instr);
}
