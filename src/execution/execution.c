/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/30 15:56:58 by fdessoy-         ###   ########.fr       */
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
	// int i = 0;
	// while (head) // this is for debugging
	// {
	// 	printf("token: [%i][%s] type: [%i]\n", i, head->value, head->type);
	// 	head = head->next;
	// }

	if (data->nb_cmds == 1 && !search_token_type(token, PIPE))
		data->status = single_execution(data, token, env_ll);
	else
		data->status = multiple_cmds(data, token, env_ll);
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
 * end the whole program.
 */
void	piped_execution(t_data *data, t_env **envll, char *instruction, int child)
{
	static char	*file;
	int			redirect_flag;

	redirect_flag = 0;
	if (!ft_strcmp(instruction, "<") || !ft_strcmp(instruction, ">"))
	{
		if (!ft_strcmp(instruction, ">")) // HEREDOC and APPEND needed later
			redirect_flag = REDIRECT_OUT;
		else
			redirect_flag = REDIRECT_IN;
		file = find_file(instruction, redirect_flag);
	}
	dup_fds(data, child, redirect_flag, file);
	close(data->pipe_fd[1]);
	if (checking_access(data, instruction) != 0)
	{
		free_data(data, NULL, envll, NULL);
		exit(FAILURE);
	}
	ft_exec(data, instruction, redirect_flag);
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
void	ft_exec(t_data *data, char *line, int redirect) // child is here for debugging
{
	static char	*path;
	char		**commands;
	
	if (redirect != 0)
		commands = parse_instruction(line, redirect);
	else
		commands = ft_split(line, ' ');
	if (!commands)
	{
		free_array(commands);
		free_data(data, NULL, &data->envll, NULL);
		exit (-1);
	}
	if (ft_strchr(commands[0], '/') == NULL)
		path = loop_path_for_binary(commands[0], data->binary_paths);
	else
		path = abs_path(commands[0]);
	if (!path)
	{
		free_data(data, NULL, &data->envll, commands);
		exit (1);
	}
	if (execve(path, commands, data->env) == -1)	
	{
		perror("execve");
		free_data(data, path, &data->envll, commands);
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
char	**parse_instruction(char *instruction, int redirect_flag)
{
	char	**array_instruction;
	char	*parsed_cmd;
	int		index;

	array_instruction = ft_split(instruction, ' ');
	if (!array_instruction)
		return (NULL);
	parsed_cmd = ft_strdup("");
	if (!parsed_cmd)
		return (NULL);
	if (redirect_flag == REDIRECT_OUT)
		index = 0;
	else
		index = 2;
	parsed_cmd = redirect_out(array_instruction, parsed_cmd, redirect_flag, index);
	free_array(array_instruction);
	array_instruction = ft_split(parsed_cmd, ' ');
	if (!array_instruction)
	{
		free_array(array_instruction);
		return (NULL);
	}
	return (array_instruction);
}

/**
 * This is just a loop inside the parse_instruction(). Mainly done for school
 * norm reasons.
 */
char	*redirect_out(char **array, char *instruction, int flag, int index)
{
	char *tmp;

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

	// this fucks up everything
	// i = 0;
	// while (i < data->nb_cmds)
	// {
	// 	waitpid(pids, &status, 0);
	// 	i++;
	// }