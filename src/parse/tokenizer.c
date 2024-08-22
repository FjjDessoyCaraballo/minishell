/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 13:34:27 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks the type of a given token and returns the corresponding status.
 * 
 * @param token The token to be checked.
 * @param current_token The current token being processed.
 * @param data The data structure containing info about the shell state.
 * 
 * @return 0 if the token type is found, 1 otherwise.
 */
int chunky_checker(char *token, t_token *current_token, t_data *data)
{
    if (check_builtin(token, current_token, data) == 0)
        return (0);
    if (check_echo_flag(token, current_token, data) == 0)
        return (0);
    if (check_flag(token, current_token, data) == 0)
        return (0);
    if (check_pipe(token, current_token, data) == 0)
        return (0);
    if (check_redirect(token, current_token, data) == 0)
        return (0);
    if (check_command(token, current_token, data) == 0)
        return (0);
    if (check_argument(token, current_token, data) == 0)
        return (0);
    
    printf("couldn't find type for token\n");
    return (1);
}

t_token *create_and_link_next_token(t_token *current_token, t_data *data)
{
    t_token *new_token = init_token();// Create and initialize the next token
    current_token->next = new_token;
    new_token->prev = current_token;
    data->id++;// Increment the id in the data structure
    return new_token;// Return the newly created token as the new current_token
}

t_token *initialize_tokens(t_data *data)
{
    t_token *first_node = init_token();
    data->current_token = first_node;
    data->prev_token = NULL;
    data->new_str = NULL;
    data->tok_str = NULL;
    return first_node;
}

/**
 * Performs line tokenization on the input data.
 * 
 * This function takes a pointer to a t_data structure as input, which contains the line to be tokenized.
 * It initializes the tokenization process, splits the line into tokens, and checks each token type.
 * If any token type check fails, the function returns 1. Otherwise, it returns 0.
 * 
 * @param data A pointer to a t_data structure containing the line to be tokenized.
 * @return 0 on successful tokenization, 1 on failure.
 */
int line_tokenization(t_data *data)
{
    data->first_node = initialize_tokens(data);
    data->tok_res = ft_strtok(data->line_read, data->deli, data, data->current_token);
    data->tok_str = NULL;
    while (data->tok_res != NULL)
    {
        data->current_token->id = data->id;
        data->current_token->prev = data->prev_token;
        data->tok_str = ft_strdup(data->tok_res);
        if (chunky_checker(data->tok_str, data->current_token, data) == 1)
            return (1);
        if (data->tok_res != NULL)
        {
            data->current_token = create_and_link_next_token(data->current_token, data);
            data->prev_token = data->current_token->prev;
        }
        data->tok_res = ft_strtok(NULL, data->deli, data, data->current_token);
    }
    data->token = data->first_node;
    return (0);
}
