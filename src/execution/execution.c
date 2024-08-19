/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 18:47:54 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int	token_printer(t_token *token)
// {
// 	t_token *head;
	
// 	head = token;
// 	while (head != NULL)
// 	{
// 		dprintf(2, "[%s][%i]\n", head->value, head->type);
// 		head = head->next;
// 	}
// 	head = NULL;
// 	return (SUCCESS);
// }

/**
 * Executio  and execution prepping are just the same function broke
 * into two parts for norminetting reasons. These functions mainly deal
 * with the general execution logic: separating built-ins execution from
 * all the other executions (piped, piped with redirections, just
 * redirection...). In here we fork and make the parent wait for execution
 * of the children. Another important part of it is the cl_to_array() that
 * turns our tokens into a 
 */
int    execution(t_data *data, t_env **env_ll)
{
    t_token    *token;

	token = data->token;
	data->nb_cmds = count_token(token, COMMAND);
	// token_printer(token);
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (!find_token(token, PIPE) && !find_token(token, RED_IN)
		&& !find_token(token, RED_OUT) && !find_token(token, HEREDOC)
		&& !find_token(token, APPEND) && !find_token(token, COMMAND))
		data->status = built_ins(data, token, env_ll);
	else
		data->status = execution_prepping(data, token, env_ll);
	return (data->status);
}

int	execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		**cmd_a;
	
	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (FAILURE);
	
	data->status = forking(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

int		forking(t_data *data, t_env **env_ll, char **all_cmds, pid_t pids)
{
	data->index = 0;
	while (data->index < data->nb_cmds)
	{
		if (data->piped == true)
		{
			if (pipe(data->pipe_fd) == -1)
				return (err_msg(NULL, "Broken pipe\n", 141));
		}
		pids = fork();
		if (pids < 0)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
			return (err_msg(NULL, "Failed to fork\n", -1));
		}
		if (pids == 0)
			child_execution(data, env_ll, all_cmds[data->index], data->index);
		else if (data->piped == true)
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
 * RETURN VALUES: child_execution() does not return anything as it is just a pathway
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
void	child_execution(t_data *data, t_env **env_ll, char *instr, int child)
{
	char		**cmd_array;

	
	// free_token(data->token);
	// free_ll(*env_ll);
	cmd_array = ft_split(instr, ' ');
	if (!cmd_array)
	{
		free_data(data, NULL, NULL);
		exit (err_msg(NULL, MALLOC, -1));
	}
	dup_fds(data, child, cmd_array);
	if (data->redirections == true)
	{
		cmd_array = parse_instruction(data, cmd_array);
		if (!cmd_array || !*cmd_array)
		{
			free_array(cmd_array);
			free_data(data, NULL, NULL);
			exit (err_msg(NULL, MALLOC, -1));
		}
	}
	ft_exec(data, env_ll, cmd_array);
}
// static void	line_printer(char **array)
// {
// 	int i = 0;

// 	while (array[i])
// 	{
// 		dprintf(2, "array[%i]: %s\n", i, array[i]);//debug
// 		i++;
// 	}
// }

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
void	ft_exec(t_data *data, t_env **env_ll,  char **cmd_array)
{
	static char	*path;

	data->env = env_arr_updater(env_ll);
	if (!data->env)
		exit (1);
	if (ft_strchr(cmd_array[0], '/') == NULL)
	{
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
		if (!path)
		{
			free_data(data, NULL, cmd_array);
			exit(err_msg(cmd_array[0], NO_EXEC, 127));
		}
	}
	if (!path)
	{
		if (execve(cmd_array[0], cmd_array, data->env) == -1)	
		{
			free_data(data, NULL, cmd_array);
			exit(err_msg(cmd_array[0], NO_EXEC, 127));
		}
	}
	if (execve(path, cmd_array, data->env) == -1)	
	{
		free_data(data, path, cmd_array);
		exit(err_msg(cmd_array[0], NO_EXEC, 127));
	}
}




