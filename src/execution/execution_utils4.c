/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:41:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/06 14:48:10 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
 * function returns NULL.dprintf(2, "we got here\n");
 */
char	**parse_instruction(t_data *data, char **cmd_array)
{
	int	index;
	int	len;
	char **parsed_array;
	
	index = 0;
	len = 0;
	while (cmd_array[index])
	{
		if (!ft_strcmp(cmd_array[index], ">") || !ft_strcmp(cmd_array[index], "<"))
			index++;
		len++;
		index++;
	}
	parsed_array = remove_redirect(data, cmd_array, len);
	if (!parsed_array)
	{
		dprintf(2, "\n\n WE SHOULD NOT GET HERE!!! 404 \n\n");
		free_array(cmd_array);
		return (NULL);
	}
	free_array(cmd_array);
	return (parsed_array);
}

/**
 * This function is responsible for taking out the redirection character
 * of the whole array, leaving just command, flags and arguments.
 * 
 * 	we are getting the whole array and we need to take out the
	redirection and change order of stuff if it is an output
	redirection.  Otherwhise we will feed the wrong arguments.
 */
char	**remove_redirect(t_data *data, char **array, int len)
{
	static char	**parsed_array;
	int			array_index;
	int			index;
	static char	*cwd;

	parsed_array = (char **)malloc(sizeof(char *) * (len + 1));
	if (!parsed_array)
		return (NULL);
	// cwd = check_absolute_path(array);
	cwd = getcwd(NULL, 0);
	index = 0;
	array_index = 0;
	data->index = 0;
	data->index++;
	while (array[array_index])
	{
		if (!ft_strcmp(array[array_index], ">"))
			array_index++;
		parsed_array[index] = ft_strdup(array[array_index]);
		if (!parsed_array[index])
			return (NULL);
		index++;
		array_index++;
	}
	dprintf(2, "cwd: %s\n", cwd);
/**
 * use access() to see if its a file or executable to define who goes first
 */
/*	while (array[i])
	{

	}
	if (file_or_executable(array) == FILE || file_or_executable(array) == EXECUTABLE)
	{
		if (file_or_executable(array) == FAILURE)
			|| file_or_executable(array) == REDIRECT)
			index++;
		
	}

*/
	parsed_array[index] = NULL;
	return (parsed_array);
}
/*
int	file_or_executable(char *element)
{
	int	i;

	i = 0;

		if (!ft_strcmp(element, ">")
			|| !ft_strcmp(element, "<"))
			return (REDIRECT);
		if (!access(element, F_OK))
		{
			if (!access(element, X_OK))
				return (EXECUTABLE);
			return (FILE);
		}
		else
		element;
		i++;
	return (FAILURE);
}
*/
/*************************************************************
 ************************* DUMP ******************************
 *************************************************************/

/**
//  * This function takes care of executing commands with no child_processes.
//  * USAGE: %> cmd -flag argument OR %> cmd argument -flag
//  */
// int	single_execution(t_data *data, t_token *token, t_env **env_ll)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	status = 0;
// 	if (pid < 0)
// 	{
// 		free_ll((*env_ll));
// 		free_data(data, NULL, env_ll, NULL);
// 		perror("fork");
// 		data->status = -1;
// 		return (-1);
// 	}
// 	else if (pid == 0)
// 		single_child(data, token, env_ll);
// 	else
// 		status = single_parent(pid, status);
// 	return (status);
// }

// void	single_child(t_data *data, t_token *token, t_env **env_ll)
// {
// 	static char	**command_array;
// 	static char	**execution_array;
// 	static char	**env;
// 	char		*path;
// 	// t_token	*head;
// 	// int		redir_flag;

// 	// head = token;
// 	// redir_flag = 0;
// 	// command_array = ttad(token, 0);
// 	command_array = cl_to_array(token);
// 	execution_array = ft_split(command_array[0], ' ');
// 	free_array(command_array);
// 	// if (count_token(token, RED_IN) > 0 || count_token(token, RED_IN) > 0
// 	// 	|| count_token(token, RED_IN) > 0 || count_token(token, RED_IN) > 0)
// 	// {
// 	// 	if (find_token(token, RED_IN) || find_token(token, RED_OUT)
// 	// 		|| find_token(token, HEREDOC) || find_token(token, APPEND))
// 	// 		redir_flag = 1;
// 	// 		command_array = parse_instruction(command_array);
// 	// }
// 	/**
// 	 * 
// 	 * this is where we are going to deal with the redirection of the command
// 	 * we need to take care of heredoc, append, red out and red in
// 	 * 
// 	 */
// 	// if (redir_flag == 0)
// 	// else
// 	// 	path = ft_strsjoin(token->path, command_array[0], '/');
// 	path = ft_strsjoin(token->path, token->value, '/');
// 	env = env_arr_updater(env_ll);
// 	if (execve(path, execution_array, env) == -1)
// 	{
// 		free_array(env);
// 		free_array(execution_array);
// 		free_data(data, path, env_ll, command_array);
// 		exit (127);
// 	}
// }

// int	single_parent(pid_t pid, int status)
// {
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	else
// 		return (-1);
// }
