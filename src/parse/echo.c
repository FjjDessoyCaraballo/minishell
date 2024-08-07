/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:30:12 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/07 00:21:57 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *concatenate_echo_args(t_token *current_token, const char *delimiters, t_data *data)
{
    char *concatenated_args = NULL;
    char *token;
    int first_token = 1;

    while ((token = ft_strtok(NULL, delimiters, data, current_token)) != NULL)
    {
        if (data->error == 4)
            return NULL;
        char *temp;

        if (concatenated_args)
        {
            // Only add a space if it's not the first token
            if (!first_token)
            {
                temp = ft_strjoin(concatenated_args, " ");
                free(concatenated_args);
                concatenated_args = ft_strjoin(temp, token);
                free(temp);
            }
            else
            {
                temp = ft_strdup(token);
                free(concatenated_args);
                concatenated_args = temp;
            }
        }
        else
        {
            concatenated_args = ft_strdup(token);
        }
        first_token = 0; // After the first token, subsequent tokens should be prefixed with a space
    }
    return concatenated_args;
}


/**
 * echoing - This function is responsible for processing the echo command and its 
 * arguments. It takes in a pointer to the current token, a pointer to the previous 
 * token, a string of delimiters, and a pointer to the data structure. It concatenates 
 * the arguments and stores the result in a new token.
 *
 * @param current_token A pointer to the current token in the linked list.
 * @param prev_token A double pointer to the previ ous token in the linked list.
 * @param delimiters A string of delimiters to be used by ft_strtok.
 * @param data A pointer to the data structure containing various information about 
 * the shell.
 *
 * @return This function does not return anything.
 */
void echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data)
{
    // Call the function to concatenate the arguments
    char *concatenated_args = concatenate_echo_args(current_token, delimiters, data);
    if (concatenated_args == NULL && data->status == 4)// Check if the concatenation was successful
        return;// If the concatenation failed and the status is set to 4, return
    
    current_token->next = init_token();// Create a new token and initialize it
    current_token->next->prev = current_token;
    *prev_token = current_token;// Update the previous token pointer
    current_token = current_token->next; // Move the current token pointer to the newly created token
    current_token->type = ARGUMENT;// Set the type and value of the new token
    current_token->value = concatenated_args;
}


