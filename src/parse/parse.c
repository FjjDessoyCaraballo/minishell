/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:49:16 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/05 10:39:26 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void enter_file(int fd)// this is what happens inside the file as we add texts to it
{
    char    buffer[1024];
    ssize_t bytes_read;

    while (1)
    {
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0)
        {
            perror("read");
            break;
        }
        buffer[bytes_read] = '\0'; // Null-terminate the string
        if (strncmp(buffer, "END", 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0'))
            break ;
        if (write(fd, buffer, bytes_read) != bytes_read)
            break ;
    }
    close(fd);//closes file ofc
}

int handle_output_redirection(t_token *current_token, t_token *next_token)
{
    int fd;
    
    if (current_token->type == RED_OUT)
    {
        fd = open(next_token->value, O_WRONLY | O_CREAT | O_TRUNC, 0664);
        if (fd < 0)
            return (FAILURE);
        enter_file(fd);
        close(fd);
        return (SUCCESS);
    }
    else if (current_token->type == APPEND)
    {
        fd = open(next_token->value, O_WRONLY | O_CREAT | O_APPEND, 0664);
        if (fd < 0)
            return (FAILURE);
        enter_file(fd);
        close(fd);
        return (SUCCESS);
    }
    // close(fd);
    return (FAILURE);
}


int handle_input_redirection(t_token *token, t_token *next_token)
{
    int fd;

    if (ft_strcmp(token->value, "<") == 0)
    {
        fd = open(next_token->value, O_RDONLY);
        close (fd); 
        return (0);
    }
    return (FAILURE);
}

int parse_token(t_token *token)
{
    t_token *current_token = token;

    while (current_token != NULL)
    {
        if ((current_token->type == RED_OUT || current_token->type == APPEND)
            && current_token->next != NULL && current_token->next->value != NULL)
        {
            if (current_token->prev == NULL)
            {
                if (current_token->next->type == ARGUMENT)
                    handle_output_redirection(current_token, current_token->next);
                else
                    return (44);
            }
            else
            {
                if (current_token->next->type == ARGUMENT && current_token->prev->type == PIPE)
                    handle_output_redirection(current_token, current_token->next);
                else
                    return (44);
            }
        }
        else if ((current_token->type == RED_IN || current_token->type == HEREDOC)
            && current_token->next != NULL)
        {
            if (current_token->prev == NULL)
            {
                if (current_token->next->type == ARGUMENT)
                    handle_input_redirection(current_token, current_token->next);
                else
                    return (44);
            }
            else
            {
                if (current_token->next->type == ARGUMENT && current_token->prev->type == PIPE)
                    handle_input_redirection(current_token, current_token->next);
                else
                    return (44);
            }
        }
        current_token = current_token->next;
    }
    return (SUCCESS);
}
