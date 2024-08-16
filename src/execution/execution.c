/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/16 13:38:13 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/****************************************/
/************* PRINTERS *****************/
/****************************************/
static void	line_printer(char **array)
{
	int i = 0;

	while (array[i])
	{
		dprintf(2, "array[%i]: %s\n", i, array[i]);//debug
		i++;
	}
}

static int	token_printer(t_token *token)
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
}

int    execution(t_data *data, t_env **env_ll)
{
    t_token    *token;

	token = data->token;
	data->nb_cmds = count_token(token, PIPE) + 1;
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		return (FAILURE);
	token_printer(token);
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (token->type == BUILTIN && !find_token(token, PIPE))
		data->status = built_ins(data, token, env_ll);
	else
		data->status = execution_prepping(data, env_ll, token);

	return (data->status);
}

/**
 * execution_prepping() extract the information of our tokens into an array
 * and we start forking for the children. Here is also where the parent waits
 * for all of the children to be done with their executions.
 */
int	execution_prepping(t_data *data, t_env **envll, t_token *token)
{
	static pid_t	pids;
	static char		**cmd_a;
	
	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (FAILURE);
	data->status = forking(data, envll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

int		forking(t_data *data, t_env **envll, char **all_cmds, pid_t pids)
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
			if (data->piped == true)
			{
				close(data->pipe_fd[0]);
				close(data->pipe_fd[1]);
			}
			return (err_msg(NULL, "Failed to fork\n", -1));
		}
		if (pids == 0)
			child_execution(data, envll, all_cmds[data->index], data->index);
		else if (data->piped == true)
		{	
			close(data->pipe_fd[1]);
			if (data->index > 0)
				close(data->read_end);
			data->read_end = data->pipe_fd[0];
		}
		data->index++;
	}
	return (0);
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
void	child_execution(t_data *data, t_env **envll, char *instr, int child)
{
	char		**cmd_array;
	t_token		*token;

	cmd_array = ft_split(instr, ' ');
	if (!cmd_array)
	{
		free_data(data, NULL, NULL);
		exit (err_msg(NULL, MALLOC, -1));
	}
	data->here_doc = false;
	line_printer(cmd_array);
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
	token = ft_builtin_exec(cmd_array, data->token);
	if (token)
		exit(built_ins(data, token, envll));
	ft_exec(data, cmd_array);
}

// bool	is_heredoc_last(char **array, t_token *token)
// {
// 	int		i;
// 	t_token	*tmp;

// 	i = 0;
// 	while (array[i])
// 	{
// 		tmp = token;
// 		while (tmp)
// 		{
// 			if (!ft_strcmp(array[i], tmp->value))
// 			{
// 				if (tmp->type == BUILTIN)
// 					return (true);
// 			}
// 			tmp = tmp->next;
// 		}
// 		i++;
// 	}
// 	tmp = NULL;
// 	return (false);
// }