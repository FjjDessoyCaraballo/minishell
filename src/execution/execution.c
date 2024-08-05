/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 10:01:23 by fdessoy-         ###   ########.fr       */
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
	data->nb_cmds = how_many_children(token);
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
	data->status = child_action(data, env_ll, data->cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(data->cmd_a);
	return (WEXITSTATUS(data->status));
}

int	child_action(t_data *data, t_env **env_ll, char **all_cmds, int pids)
{
	data->index = 0;
	while (data->index < data->nb_cmds)
	{
		if (pipe(data->pipe_fd) == -1)
			return (err_msg("Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
			return (err_msg("Failed to fork\n", -1));
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
void	piped_execution(t_data *data, t_env **envll, char *instr, int child)
{
	static char	*file;
	int			redirect_flag;

	redirect_flag = 0;
	if (!ft_strcmp(instr, "<") || !ft_strcmp(instr, ">"))
	{
		if (!ft_strcmp(instr, ">")) // HEREDOC and APPEND needed later
			redirect_flag = REDIRECT_OUT;
		else
			redirect_flag = REDIRECT_IN;
		file = find_file(instr, redirect_flag);
	}
	dup_fds(data, child, redirect_flag, file);
	close(data->pipe_fd[1]);
	if (checking_access(data, instr) != 0)
	{
		free_data(data, NULL, envll, NULL);
		exit(FAILURE);
	}
	ft_exec(data, instr, redirect_flag);
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
