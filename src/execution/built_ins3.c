/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstorey <lstorey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:27:01 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/02 17:38:35 by lstorey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void alphabetical_printer(char **env_array)
{
    int i;
    int j;
    char c;

    i = 0;
    j = 0;
    c = 'A';

    while (c < 91)
    {
        while (env_array[i])
        {
            if (env_array[i][j] == c)
            {
                printf("declare -x ");
                printf("%s\n", env_array[i]);
                
            }
            i++;
        }
        i = 0;
        c++;
    }
}
