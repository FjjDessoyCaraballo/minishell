/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/24 09:26:52 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;

Execution should happen within child process, otherwise it quits the whole thang.
Therefore, iteration might be neccessary for either single execution or builtin
*/	
static int	token_printer(t_token *token)
{
	t_token *head;
	
	head = token;
	while (head != NULL)
	{
		printf("[%s][%i]\n", head->value, head->type);
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}

int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;
	
	token = data->token;
	token_printer(token);
	if (how_many_children(data, token) == 1 && !search_token_type(token, PIPE))
		data->status = single_execution(data, token, env_ll);
	if (how_many_children(data, token) > 1 && search_token_type(token, PIPE))
	{
		// token = find_token(token, COMMAND);
		data->status = multiple_cmds(data, token, env_ll);
	}
	data->status = built_in_or_garbage(data, env_ll, token);
	if (data->status != 0)
		return (data->status);
	return (148);
}


/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
int	multiple_cmds(t_data *data, t_token *token, t_env **env_ll)
{
	int		i;
	pid_t	pids;
	int		status;
	char	**all_cmds;
	char	**env;
	
	i = 0;
	all_cmds = cl_to_array(data, token);
	if (!all_cmds)
		return (FAILURE);
	while (i < data->nb_cmds)
	{
		data->env = env_arr_updater(env_ll);
		if (!data->env)
			return (FAILURE);
		if (pipe(data->pipe_fd) == -1)
			return (err_pipes("Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
			return (err_pipes("Failed to fork\n", -1));
		}
		if (pids == 0) // child
			piped_execution(data, all_cmds[i], i);
		else // parent
		{
			close(data->pipe_fd[1]);
			data->read_end = data->pipe_fd[0];
			waitpid(pids, &status, 0);
		}
		i++;
	}
	return (SUCCESS);
}
/**
 * AT THIS POINT WE WILL HAVE 
 */
void	piped_execution(t_data *data, char *instruction, int child)
{
	char	*path;
	char	*file;
	char	**command_array;
	int		redirect_flag;

	redirect_flag = 0;
	file = NULL;
	if (!ft_strcmp(instruction, "<") || !ft_strcmp(instruction, ">"))
	{
		if (!ft_strcmp(instruction, ">"))
			redirect_flag = REDIRECT_OUT;
		else
			redirect_flag = REDIRECT_IN;
		file = find_file(instruction, redirect_flag); // NEED TO FIND A NEW WAY TO FILTER OUT
		filter_redirect(data, instruction, child, file);
	}
	else
		dup_fds(data, child, 0, file);
	if (redirect_flag != 0) // need to parse out the redirects
	{
		command_array = parse_instruction(instruction, redirect_flag); // continue from here
		if (execve(path, command_array, data->env) == -1)	
		{
			free_array(command_array);
			free_array(data->env);
			free_data(data, path, NULL, NULL);
			exit (127);
		}
	}
	else // this can be its own function
	{
		command_array = ft_split(instruction, ' ');	
		if (checking_access(data, instruction) != 0)
		{
			// free stuff
			free_array(command_array);
			free_array(data->env);
			free_data(data, NULL, data->envll, NULL);
			exit(127);
		}
		if (execve(path, command_array, data->env) == -1)	
		{
			free_array(command_array);
			free_array(data->env);
			free_data(data, path, NULL, NULL);
			exit (127);
		}
	}
}

/**
 * Its necessary to know which redirection we have here and give back the
 * array organized in the usual fashion of "cmd -flag" for execution.
 */
char	**parse_instruction(char *instruction, int redirect_flag)
{
	char	**result;
	int		i;

	result = ft_split(instruction, ' '); // I STOPPED HERE
}

/**
 * Here we want to filter out the file, if there is one. At this point we
 * should surely have an infile, and maybe an outfile. It is not completely
 * necessary that we have an outfile, because when we open fd_out in the data
 * structure there will be an option to create a file of the users choosing.
 * This does not mean that the redirection will work without an argument, so
 * it is necessary that the user has inputted a name of a file to be created.
 */
char	*find_file(char *instruction, int redirect_flag)
{
	char	*file;
	char	**filter;

	filter = ft_split(instruction, ' ');
	if (!filter)
		return (NULL);
	if (redirect_flag == REDIRECT_IN)
	{
		file = ft_strdup(filter[1]);
		free_array(filter);
		return (filter[2]);
	}
	else if (redirect_flag == REDIRECT_OUT)
	{
		file = ft_strdup(filter[3]);
		free_array(filter);
		return (file);
	}
	return (NULL);
}

/**
 * This function rearrenges the command array in case there are redirects.
 * The flag one is used to identify input redirect;
 * The flag zero is used to identify output redirect;
 */
void	filter_redirect(t_data *data, char *instruction, int child, char *file)
{
	if (!ft_strcmp(instruction, "<"))
		dup_fds(data, child, 1, file);
	else if (ft_strcmp(instruction, ">"))
		dup_fds(data, child, 1, file);
}



