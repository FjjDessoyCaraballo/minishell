/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:19:20 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/24 17:45:05 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void dup_fds(t_data *data, int input_fd, int output_fd, int fd_flag, char *file)
{
    if (fd_flag == REDIRECT_IN && file)
    {
        open_fdin(data, file);
        dup2(data->fd_in, STDIN_FILENO);
        close(data->fd_in);
    }
    else if (fd_flag == REDIRECT_OUT && file)
    {
        open_fdout(data, file);
        dup2(data->fd_out, STDOUT_FILENO);
        close(data->fd_out);
    }
	if (input_fd != -1)
    {
        if (dup2(input_fd, STDIN_FILENO) == -1)
        {
            perror("dup2 input_fd");
            close_fds(data);
            exit(EXIT_FAILURE);
        }
        close(input_fd);
    }

    if (output_fd != -1)
    {
        if (dup2(output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 output_fd");
            close_fds(data);
            exit(EXIT_FAILURE);
        }
        close(output_fd);
    }
}

void open_fdin(t_data *data, char *infile)
{
    errno = 0;
    data->fd_in = open(infile, O_RDONLY);
    if (data->fd_in == -1)
    {
        perror("open fd_in");
        close_fds(data);
        exit_child(infile, errno);
    }
}

void open_fdout(t_data *data, char *outfile)
{
    errno = 0;
    data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
    if (data->fd_out == -1)
    {
        perror("open fd_out");
        close_fds(data);
        exit_child(outfile, errno);
    }
}

void close_fds(t_data *data)
{
    if (data->pipe_fd[0] != -1)
        close(data->pipe_fd[0]);
    if (data->pipe_fd[1] != -1)
        close(data->pipe_fd[1]);
    if (data->fd_in != -1)
        close(data->fd_in);
    if (data->fd_out != -1)
        close(data->fd_out);
    if (data->read_end != -1)
        close(data->read_end);
}

void exit_child(char *file, int err_code)
{
    ft_putstr_fd(file, 2);
    ft_putstr_fd(": ", 2);
    if (err_code == ENOENT)
        ft_putstr_fd("No such file or directory\n", 2);
    else if (err_code == EACCES)
    {
        ft_putstr_fd("Permission denied\n", 2);
        err_code = 1;
    }
    else if (err_code == EISDIR)
    {
        ft_putstr_fd("Is a directory\n", 2);
        err_code = 1;
    }
    exit(err_code);
}
