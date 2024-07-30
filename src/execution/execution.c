/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 22:17:59 by fdessoy-         ###   ########.fr       */
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
/*static int	token_printer(t_token *token)
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
}*/

int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;

	token = data->token;
	data-> nb_cmds = how_many_children(token);
	// t_token *head = token;
	// while (head) // this is for debugging
	// {
	// 	printf("token: [%s] type: [%i]\n", head->value, head->type);
	// 	head = head->next;
	// }
	if (data->nb_cmds == 1 || find_token(token, BUILTIN) != NULL)
	{
		if (data->nb_cmds == 1)
			data->status = single_execution(data, token, env_ll);
		else
			data->status = built_in_or_garbage(data, env_ll, token); // builtins will be thrown into single execution later
	}
	else if (data->nb_cmds > 1)
		data->status = multiple_execution(data, token, env_ll);
	return (data->status);
}

/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
int	multiple_execution(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;

	data->cmd_a = cl_to_array(token);
	if (!data->cmd_a)
		return (FAILURE);
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		return (FAILURE);
	data->status = piping(data, env_ll, data->cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(data->cmd_a);
	return (WEXITSTATUS(data->status));
}

int	piping(t_data *data, t_env **env_ll, char **all_cmds, int pids)
{
	data->index = 0;
	while (data->index < data->nb_cmds)
	{
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
			piped_execution(data, env_ll, all_cmds[data->index], data->index);
		else // parent
		{
			close(data->pipe_fd[1]);
			if (data->index > 0)
				close(data->read_end);
			data->read_end = data->pipe_fd[0];
		}
		data->index++;
	}
	return (data->index);
}

/**
 * The piped execution is where the child processes go. Here we will check for
 * redirections to know if the user wants the output/input to be redirected from/to
 * a file.
 * 
 * RETURN VALUES: piped_execution() does not return anything as it is just a pathway
 * to the final part of the execution in ft_exet().
 * 
 * DETAILS: at this point we may use exit() function without worrying that we will
 * end the whole program. Also, at this point we are working with fully parsed out
 * strings, our only concern should be if files/commands don't exist. Examples of
 * instruction:
 * "< infile cat"
 * "cat > outfile"
 * "ls -la Makefile"
 * "> outfile"
 * "<< END"
 */
void	piped_execution(t_data *data, t_env **envll, char *instruction, int child)
{
	static char	*file;
	char		**cmd_array;
	int			redirect_flag;

	redirect_flag = 0;
	data->index = 0;
	cmd_array = ft_split(instruction, ' ');
	while (cmd_array[data->index])
	{
		if (!ft_strcmp(cmd_array[data->index], "<")
			|| !ft_strcmp(cmd_array[data->index], ">"))
		{
			if (!ft_strcmp(cmd_array[data->index], ">"))
			{
				file = ft_strdup(cmd_array[data->index + 1]);
				redirect_flag = REDIRECT_OUT;
			}
			else
			{
				file = ft_strdup(cmd_array[data->index + 1]);
				redirect_flag = REDIRECT_IN;
			}
		}
		data->index++;
	}
	if (checking_access(data, instruction) != 0 || !file)
	{
		free_array(data->cmd_a);
		free_array(cmd_array);
		free_array(data->binary_paths);
		free_ll(*envll);
		exit(FAILURE);
	}
	dprintf(1, "redirect flag is: %i\n", redirect_flag);
	dprintf(1, "file is: %s\n", file);
	dup_fds(data, child, redirect_flag, file);
	close(data->pipe_fd[1]);
	ft_exec(data, cmd_array, redirect_flag);
}

/**
 * This is the second part of the execution where we are going to
 * check if we have the redirection flag (int redirect) and we are
 * parsing the commands differently if we do.
 * 
 * Redirections here take an even more strict definition:
 * - "%> cat << EOF | cat > outfile"
 * 
 * Therefore, in here, redirections will be strictly "<" and ">", while
 * HERE_DOC and APPEND will have explicit naming because they are able
 * to take arguments beforehand.
 * 
 * [placeholder for more documentation]
 */
void	ft_exec(t_data *data, char **cmd_array, int redirect) // child is here for debugging
{
	static char	*path;
	
	if (redirect != 0)
		cmd_array = parse_instruction(cmd_array, redirect); // this is not working
	if (!cmd_array)
	{
		free_array(cmd_array);
		free_data(data, NULL, &data->envll, NULL);
		exit (-1);
	}
	if (ft_strchr(cmd_array[0], '/') == NULL)
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
	else
		path = abs_path(cmd_array[0]);
	if (!path)
	{
		free_data(data, NULL, &data->envll, cmd_array);
		exit (1);
	}
	if (execve(path, cmd_array, data->env) == -1)	
	{
		perror("execve");
		free_data(data, path, &data->envll, cmd_array);
		exit(127);
	}
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
char	**parse_instruction(char **cmd_array, int redirect_flag)
{
	int	index;
	int	len;
	char **parsed_array;

	len = 0;
	index = 0;
	while (cmd_array[index])
	{
		if (!ft_strcmp(cmd_array[index], ">") || !ft_strcmp(cmd_array[index], "<"))
			index++;
		len++;
		index++;
	}
	parsed_array = remove_redirect(cmd_array, len, redirect_flag);
	if (!parsed_array)
	{
		free_array(cmd_array);
		return (NULL);
	}
	free_array(cmd_array);
	return (parsed_array);
}

/**
 * This function is responsible for taking out the redirection character
 * of the whole array, leaving just command, flags and arguments.
 */
char	*remove_redirect(char **array, int len, int flag)
{
	// we are getting the whole array and we need to take out the
	// redirection and change order of stuff if it is an output
	// redirection.  Otherwhise we will feed the wrong arguments.
	char 	**parsed_array;
	int		index;

	index = 0;
	parsed_array = (char *)malloc(sizeof(char *) * (len + 1));
	while (array[index])
	{
		if (flag == REDIRECT_IN)
			// deal with input redirection syntax: everything after <
		else if (flag == REDIRECT_OUT)
			// deal with output redirection syntax: anything but >
		index++;
	}
	return (parsed_array);
}


/*************************************************************
 ************************* DUMP ******************************
 *************************************************************/

		// if (!ft_strcmp(array[index], '>'))
		// 	break ;
		// tmp = ft_strjoin(instruction, array[index]);
		// if (!tmp)
		// 	return (NULL);
		// free(instruction);
		// instruction = ft_strjoin(tmp, " ");
		// if (!instruction)
		// 	return (NULL);
		// free(tmp);
		// index++;

// char	**parse_instruction(char *instruction, int redirect_flag)
// {
// 	char	**array_instruction;
// 	char	*parsed_cmd;
// 	int		index;

// 	array_instruction = ft_split(instruction, ' ');
// 	if (!array_instruction)
// 		return (NULL);
// 	parsed_cmd = ft_strdup("");
// 	if (!parsed_cmd)
// 		return (NULL);
// 	if (redirect_flag == REDIRECT_OUT)
// 		index = 0;
// 	else
// 		index = 2;
// 	parsed_cmd = redirect_out(array_instruction, parsed_cmd, redirect_flag, index);
// 	free_array(array_instruction);
// 	array_instruction = ft_split(parsed_cmd, ' ');
// 	if (!array_instruction)
// 	{
// 		free_array(array_instruction);
// 		return (NULL);
// 	}
// 	return (array_instruction);
// }

	// int i = 0;
	// while (cmd_array[i])
	// {
	// 	dprintf(2, "command[%i]: %s\n", i, cmd_array[i]);
	// 	i++;
	// }

	// else
	// 	commands = ft_split(line, ' '); // gonna try to split before