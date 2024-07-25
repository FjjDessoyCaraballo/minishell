/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/24 18:18:29 by fdessoy-         ###   ########.fr       */
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
char **cl_to_array(t_token *token)
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
	if (!instruction)
		return (NULL);
    while (head) 
	{
        instruction[0] = '\0';
        while (head && head->type != PIPE) 
		{
            tmp = ft_strjoin(instruction, head->value);
			if (!tmp)
				return (NULL);
            free(instruction);
            instruction = tmp;
            tmp = ft_strjoin(instruction, " ");
			if (!tmp)
				return (NULL);
            free(instruction);
            instruction = tmp;
            head = head->next;
        }
        if (instruction[ft_strlen(instruction) - 1] == ' ')
            instruction[ft_strlen(instruction) - 1] = '\0';
        pipe_array[i++] = ft_strdup(instruction);
        if (!pipe_array[i - 1])
            return (NULL);
        if (head && head->type == PIPE)
            head = head->next;
    }
    free(instruction);
    pipe_array[i] = NULL;
    return (pipe_array);
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
	char **split_instruction;
	char *binary;

	split_instruction = ft_split(instruction, ' ');
	if (!split_instruction)
		return (NULL);
	if (!ft_strcmp(instruction, "<"))
		binary = ft_strdup(split_instruction[3]);
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

/**
 * Here we want to filter out the file, if there is one. At this point we
 * should surely have an infile, and maybe an outfile. It is not completely
 * necessary that we have an outfile, because when we open fd_out in the data
 * structure there will be an option to create a file of the users choosing.
 * This does not mean that the redirection will work without an argument, so
 * it is necessary that the user has inputted a name of a file to be created.
 * 
 * Return value: upon completion, the function will return with the name of the
 * file that was requested. In case of failure, the  function returns NULL.
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

char	*abs_path(char *command)
{
	int		i;
	char	*path;

	i = 0;
	
	while (command[i++])
	{
		if (ft_strchr(command, '/') != NULL)
			return (path = ft_strchr(command, '/'));
	}
	return (NULL);
}


// char **cl_to_array(t_token *token)
// {
// 	t_token *head;
// 	char	**pipe_array;
// 	char	*instruction;
// 	char	*tmp;
// 	int		i;
// 	int		nb_of_instructions;
	
// 	nb_of_instructions = how_many_tokens(token);
// 	pipe_array = (char **)malloc(sizeof(char *) * (nb_of_instructions + 1));
// 	if (!pipe_array)
// 		return (NULL);
// 	i = 0;
// 	head = token;
// 	instruction = ft_strdup("");
// 	if (!instruction)
// 		return (NULL);
	
// 	while (head)
// 	{
// 		while (head->type != PIPE)
// 		{
// 			printf("head->value: %s\ninstruction: %s\n", head->value, instruction);
// 			tmp = ft_strjoin(instruction, head->value);
// 			if (!tmp)
// 				return (NULL);
// 			free(instruction);
// 			instruction = ft_strjoin(tmp, " ");
// 			if (!instruction)
// 				return (NULL);
// 			free(tmp);
// 			pipe_array[i++] = ft_strdup(instruction);
// 			head = head->next;
// 		}
// 		head = head->next; // jumping over the pipe to next command
// 		free(instruction);
// 	}
// 	printf("we are getting here\n");
// 	pipe_array[i] = NULL;
// 	return (pipe_array);
// }