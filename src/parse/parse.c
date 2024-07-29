/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:49:16 by walnaimi          #+#    #+#             */
/*   Updated: 2024/07/29 22:53:27 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void enter_file(int fd)
{
    char buffer[1024];
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
        {
            break;
        }

        if (write(fd, buffer, bytes_read) != bytes_read)
        {
            perror("write");
            break;
        }
    }

    close(fd);
}

int handle_output_redirection(t_token *current_token, t_token *next_token)
{
    int fd;

    if (current_token->type == RED_OUT)
    {
        fd = open(next_token->value, O_WRONLY | O_CREAT | O_TRUNC, 0664);
        if (fd < 0)
        {
            perror("open");
            return FAILURE;
        }
        enter_file(fd);
        printf("%d\n",fd);
        //printf("\n");
        return SUCCESS;
    }
    else if (current_token->type == APPEND)
    {
        fd = open(next_token->value, O_WRONLY | O_CREAT | O_APPEND, 0664);
        if (fd < 0)
        {
            perror("open");
            return FAILURE;
        }
        enter_file(fd);
        printf("%d\n",fd);
        //printf("\n");
        return SUCCESS;
    }
    return FAILURE;
}


int handle_input_redirection(t_token *token, t_token *next_token)
{
    if (ft_strcmp(token->value, "<") == 0)
    {
        open(next_token->value, O_RDONLY);
        printf("\n"); // idk if we actually need to print this new line
        return (0);
    }
    return FAILURE;
}



int parse_token(t_token *token)
{
    t_token *current_token = token;

    while (current_token != NULL)
    {
        // Handle output redirection (">" or ">>")
        if ((current_token->type == RED_OUT || current_token->type == APPEND)
            && current_token->next != NULL && current_token->next->value != NULL)
        {
            if (current_token->prev == NULL) // No previous token
            {
                if (current_token->next->type == ARGUMENT)
                    handle_output_redirection(current_token, current_token->next);
                else
                {
                    printf("error 106\n");
                    return 44;
                }
            }
            else // Previous token exists
            {
                if (current_token->next->type == ARGUMENT && current_token->prev->type == PIPE)
                    handle_output_redirection(current_token, current_token->next);
                else
                {
                    printf("error 117\n");
                    return 44;
                }
            }
        }
        // Handle input redirection ("<" or "<<")
        else if ((current_token->type == RED_IN || current_token->type == HEREDOC)
            && current_token->next != NULL && current_token->next->value != NULL)
        {
            if (current_token->prev == NULL) // No previous token
            {
                if (current_token->next->type == ARGUMENT)
                    handle_input_redirection(current_token, current_token->next);
                else
                {
                    printf("error 132\n");
                    return 44;
                }
            }
            else // Previous token exists
            {
                if (current_token->next->type == ARGUMENT && current_token->prev->type == PIPE)
                    handle_input_redirection(current_token, current_token->next);
                else
                {
                    printf("error 142\n");
                    return 44;
                }
            }
        }
        current_token = current_token->next; // Move to the next token
    }
    return SUCCESS;
}
