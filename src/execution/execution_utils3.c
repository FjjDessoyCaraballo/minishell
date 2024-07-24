/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/24 09:42:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * COMMAND LINE TO ARRAY
 * This function takes all tokens and organize them into elements of an array
 * so that we can distribute instructions to each child process individually.
 * 
 * E.g.:
 * *************** IN COMMAND LINE ********************
 * %> ls -la | grep Makefile | cat > outfile
 * ****************************************************
 * The array will take the instructions and organize them as follows:
 * pipe_array[0] = "ls -la"
 * pipe_array[1] = "grep Makefile"
 * pipe_array[2] = "cat > outfile"
 */
char **cl_to_array(t_data *data, t_token *token)
{
	t_token *head;
	char	**pipe_array;
	char	*instruction;
	char	*tmp;
	int		i;
	int		nb_of_instructions;
	
	nb_of_instructions = how_many_tokens(token);
	pipe_array = (char **)malloc(sizeof(char *) * (nb_of_instructions + 1));
	if (!pipe_array)
		return (NULL);
	i = 0;
	head = token;
	instruction = ft_strdup("");
	while (head)
	{
		while (head->type != PIPE)
		{
			tmp = ft_strjoin(instruction, head->value);
			free(instruction);
			instruction = ft_strjoin(tmp, " ");
			free(tmp);
			head = head->next;
		}
		head = head->next; // jumping over the pipe to next command
		pipe_array[i++] = ft_strdup(instruction);
		free(instruction);
	}
	return (pipe_array);
}

int	checking_access(t_data *data, char *instruction)
{
	int		i;
	char	*binary_path;
	char	*binary;
	
	i = 0;
	binary = get_binary(data, instruction);
	while (data->binary_paths[i++])
	{
		binary_path = ft_strsjoin(data->binary_paths[i], binary, '/');
		if (!access(binary_path, F_OK))
		{
			if(!access(binary_path, X_OK))
				return (SUCCESS);
			else
			{
				ft_putstr_fd(binary, 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("command not found\n", 2);
				free(binary);
				return (FAILURE);
			}
		}
		else
			free(binary_path);
	}
	return (FAILURE);
}

/**
 * At this point we have an instruction that should follow this syntax:
 * "cmd -flag argument"
 * "cmd -flag"
 * "cmd argument"
 * "redirection infile flag"
 * "cmd redirection outfile"
 * All these cases will have spaces between them that were set manually in
 * cl_to_array function.
 */
char	*get_binary(t_data *data, char *instruction)
{
	char **split_instruction;
	char *binary;
	char *file;

	if (!ft_strcmp(instruction, "<")) // input case: cmd is the third parameter
	{
		split_instruction = ft_split(instruction, " ");
		binary = ft_strdup(split_instruction[3]);
		free_array(split_instruction);
	}
	else // normal input: cmd is the first parameter
	{
		split_instruction = ft_split(instruction, " ");
		binary = ft_strdup(split_instruction[0]);
		free_array(split_instruction);
	}
	return (binary);
}
/**
 * Frees the shell most used data allocators.
 * USAGE: pass the function the arguments it asks, otherwhise use NULL.
 * data: main data struct for the shell;
 * path: defined and allocated in the execution part;
 * env: our linked list that works as our environment pointer;
 * command_array: variable linked to the execution of single/multiple processes.
 */
void	free_data(t_data *data, char *path, t_env **env, char **command_array)
{
	free_array(data->binary_paths);
	free_ll(*env);
	if (command_array)
		free_array(command_array);
	if (path)
		free(path);
	free(data);
}