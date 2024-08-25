/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:28:13 by fdessoy-          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/25 22:41:26 by walnaimi         ###   ########.fr       */
=======
/*   Updated: 2024/08/25 23:28:55 by walnaimi         ###   ########.fr       */
>>>>>>> BrunoPosa-main
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void compare_and_print_type(int array_index, char **array, t_token *tokens)
// {
//     t_token *current = tokens;
//     while (current)
//     {
//         if (current->id == array_index)
//         {
//             if (strcmp(current->value, array[array_index]) == 0)
//             {
//                 printf("Token ID: %d, Token Value: %s, Token Type: %d\n", current->id, current->value, current->type);
//                 return;
//             }
//             else
//             {
//                 printf("Mismatch: Token Value: %s, Array Value: %s\n", current->value, array[array_index]);
//                 return;
//             }
//         }
//         current = current->next;
//     }
//     printf("No token found with ID: %d\n", array_index);
// }

void	input_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		if (!access(array[data->index + 1], F_OK))
		{
			open_fdin(data, array[data->index + 1]);
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
			if (data->piped == true)
				dup2(data->pipe_fd[1], STDOUT_FILENO);
		}
		else
			exit(err_msg(array[data->index + 1], FILE_ERROR, 2));
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	output_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 1);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	append_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 0);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	heredoc_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		if (data->piped == false)
		{
			data->fd_in = here_doc(array[data->index + 1], data);
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
		}
		else if (data->piped == true)
		{
			data->fd_in = here_doc(array[data->index + 1], data);
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
		}
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}