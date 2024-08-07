/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:30:12 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/06 16:42:51 by walnaimi         ###   ########.fr       */
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

        // Expand environment variables within the token
        char *expanded_token = expand_env_variables(token, data);

        if (concatenated_args)
        {
            // Only add a space if it's not the first token
            if (!first_token)
            {
                temp = ft_strjoin(concatenated_args, " ");
                free(concatenated_args);
                concatenated_args = ft_strjoin(temp, expanded_token);
                free(temp);
            }
            else
            {
                temp = ft_strdup(expanded_token);
                free(concatenated_args);
                concatenated_args = temp;
            }
        }
        else
        {
            concatenated_args = ft_strdup(expanded_token);
        }
        free(expanded_token); // Free the expanded token
        first_token = 0; // After the first token, subsequent tokens should be prefixed with a space
    }
    return concatenated_args;
}

void echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data)
{
    char *concatenated_args = concatenate_echo_args(current_token, delimiters, data);
    if (concatenated_args == NULL && data->status == 4)
        return;
    //printf("this catted arg is :%s\n", concatenated_args);//debug
    current_token->next = init_token();
    current_token->next->prev = current_token;
    *prev_token = current_token;
    current_token = current_token->next;
    current_token->type = ARGUMENT;
    current_token->value = concatenated_args;
}
