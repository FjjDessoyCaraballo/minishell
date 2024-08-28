/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/26 00:54:16 by walnaimi         ###   ########.fr       */
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
 * 
 * ***NOTE: I dont think we need malloc checks after strdup, as its taken care of
 *  inside the function that should save a few lines at least***
 */
char	**cl_to_array(t_token *token)
{
	t_token	*head;
	char	**pipe_array;
	int		i;

	i = 0;
	head = token;
	pipe_array = ft_calloc((count_token(token, PIPE) + 2), sizeof(char *));
	if (!pipe_array)
		return (NULL);
	while (head)
	{
		pipe_array[i] = build_instruction(&head);
		if (!pipe_array[i])
			return (free_arr_retnull(pipe_array));
		i++;
		if (head && head->type == PIPE)
			head = head->next;
		else
			break ;
	}
	pipe_array[i] = NULL;
	return (pipe_array);
}

char	*build_instruction(t_token **head)
{
	char	*instruction;
	char	*tmp;

	tmp = NULL;
	instruction = ft_strdup("");
	while ((*head) && (*head)->type != PIPE)
	{
		tmp = ft_strjoin(instruction, (*head)->value);
		free(instruction);
		if (!tmp)
			return (NULL);
		instruction = tmp;
		tmp = ft_strjoin(instruction, " ");
		free(instruction);
		if (!tmp)
			return (NULL);
		instruction = tmp;
		if (!(*head)->next || !(*head)->next->value)
			break ;
		(*head) = (*head)->next;
	}
	head = NULL;
	if (instruction[ft_strlen(instruction) - 1] == ' ')
		instruction[ft_strlen(instruction) - 1] = '\0';
	return (instruction);
}

/** checking_access() is mainly a last check for general binaries that
 * the original shell uses. If the user inputs a binary of his own making
 * it will fail this check, since it uses the paths that were stablished
 * in the environment variable.
 * 
 * USAGE: pass the struct data and the instruction and it will find the
 * binary by itself and check for existence (F_OK) and executability (X_OK)
 * 
 * RETURN VALUES: checking_access() either returns SUCCESS or FAILURE. If
 * FAILURE is returned, it means that your binary cannot be found in the
 * general concatenated paths in the environment pointers.
 */
int	checking_access(t_data *data, char *instruction)
{
	int		i;
	char	*binary_path;
	char	*binary;

	i = 0;
	binary = get_binary(instruction);
	while (data->binary_paths[i])
	{
		binary_path = ft_strsjoin(data->binary_paths[i++], binary, '/');
		if (!access(binary_path, F_OK))
		{
			if (!access(binary_path, X_OK))
			{
				free(binary);
				return (free_retstatus(binary_path, SUCCESS));
			}
			ft_putstr_fd(binary, 2);
			ft_putstr_fd(": command not found\n", 2);
			free(binary);
			return (free_retstatus(binary_path, FAILURE));
		}
		free(binary_path);
	}
	free(binary);
	return (FAILURE);
}

/**
 * At this point we have an instruction that should follow this syntax:
 * 
 * "cmd -flag argument"
 * "cmd -flag"
 * "cmd argument"
 * "redirection infile flag"
 * "cmd redirection outfile"
 * 
 * All these cases will have spaces between them that were set manually in
 * cl_to_array function.
 * 
 * RETURN VALUES: the function returns the binary upon success. If allocation
 * fails at some point, it returns NULL and frees memory used in the function.
 */
char	*get_binary(char *instruction)
{
	char		**split_instruction;
	static char	*binary;

	split_instruction = ft_split(instruction, ' ');
	if (!split_instruction)
		return (NULL);
	if (!ft_strcmp(split_instruction[0], "<"))
		binary = ft_strdup(split_instruction[2]);
	else
		binary = ft_strdup(split_instruction[0]);
	if (!binary)
	{
		free_array(split_instruction);
		return (NULL);
	}
	free_array(split_instruction);
	return (binary);
}
