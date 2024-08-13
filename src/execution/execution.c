/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/13 10:27:14 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	line_printer(char **array)
{
	int i = 0;

	while (array[i])
	{
		dprintf(2, "array[%i]: %s\n", i, array[i]);//debug
		i++;
	}
}

/*static int	token_printer(t_token *token)
{
	t_token *head;
	
	head = token;
	while (head != NULL)
	{
		dprintf(2, "[%s][%i]\n", head->value, head->type);
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}  // */

int    execution(t_data *data, t_env **env_ll)
{
    t_token    *token;

	token = data->token;
	data->nb_cmds = count_token(token, COMMAND);
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (token->type != BUILTIN)
		data->status = execution_prepping(data, token, env_ll);
	else
		data->status = built_ins(data, token, env_ll);
	return (data->status);
}

/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
int	execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		**cmd_a;

	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (FAILURE);
	data->status = piping(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

int	piping(t_data *data, t_env **env_ll, char **all_cmds, int pids)
{
	data->index = 0;
	while (data->index < data->nb_cmds)
	{
		if (pipe(data->pipe_fd) == -1)
			return (err_msg(NULL, "Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
			return (err_msg(NULL, "Failed to fork\n", -1));
		}
		data->env = env_arr_updater(env_ll);
		if (!data->env)
			return (FAILURE);
		if (pids == 0)
			piped_execution(data, env_ll, all_cmds[data->index], data->index);
		else
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
void	piped_execution(t_data *data, t_env **envll, char *instr, int child)
{
	char		**cmd_array;

	data->index = 0;
	cmd_array = ft_split(instr, ' ');
	dup_fds(data, child, cmd_array); // something wrong is happening in the middle children
	if (checking_access(data, instr, child) != 0)
	{
		free_array(cmd_array);
		free_array(data->binary_paths);
		free_ll(*envll);
		exit(FAILURE);
	}
	ft_exec(data, cmd_array);
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
void	ft_exec(t_data *data, char **cmd_array)
{
	static char	*path;

	if (data->redirections == false)
		dprintf(2, "something terrible has happened [no redirects]\n");
	if (data->redirections == true)
	{
		dprintf(2, "ok, something terrible did NOT happen\n");
		cmd_array = parse_instruction(data, cmd_array); // still not working
	}
	line_printer(cmd_array);
	if (!cmd_array || !*cmd_array)
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
