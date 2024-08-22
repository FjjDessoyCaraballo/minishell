/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:42:40 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/22 15:00:45 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Adds spaces around a single character in a string.
 *
 *  Add a space before the character if it's not the first character
 *  and the previous character is not a space.
 * 
 * Add a space after the character if it's not the last character 
 * and the next character is not a space.
 * Parameters:
 *  @param str (char *): The original string.
 *  @param new_str (char *): The new string with added spaces.
 *  @param i (int *): The index of the current character in the string.
 *  @param j (int *): The index of the current character in the new string.
 *  @param ch (char): The single character to add spaces around.
 *
 * Returns:
 *  None
 */
void add_spaces_s_c(char *str, char *new_str, int *i, int *j, char ch)
{
    int len = strlen(str);

    if (*i > 0 && !isspace(str[*i - 1]))
        new_str[(*j)++] = ' ';
    new_str[(*j)++] = ch;

    if (*i < len - 1 && !isspace(str[*i + 1]))
        new_str[(*j)++] = ' ';
}

/**
 * Adds spaces around a double character in a string.
 *
 * Add a space before the double character if it's not the first character
 * and the previous character is not a space.
 * 
 * Add a space after the double character if it's not the last character
 * and the next character is not a space.
 * 
 * Parameters:
 *  @param s (char *): The original string.
 *  @param ns(char *): The new string with added spaces.
 *  @param i (int *): The index of the current character in the string.
 *  @param j (int *): The index of the current character in the new string.
 *  @param ch (char): The double character to add spaces around.
 *
 * Returns:
 *  None
 */
void add_spaces_d_c(char *s, char *ns, int *i, int *j, char ch)
{
    int len;
    len = ft_strlen(s);

    if (*i > 0 && !isspace(s[*i - 1]))
        ns[(*j)++] = ' ';

    ns[(*j)++] = ch;
    ns[(*j)++] = ch;

    if (*i < len - 2 && !isspace(s[*i + 2]))
        ns[(*j)++] = ' ';

    (*i)++;
}

void process_characters(char *s, char *n_s, int *i, int *j, int *s_q, int *d_q)
{
    if (s[*i] == '\'' && !(*d_q))
        *s_q = !(*s_q); // Toggle single quote state
    else if (s[*i] == '"' && !(*s_q))
        *d_q = !(*d_q); // Toggle double quote state

    if (!(*s_q) && !(*d_q))
    {
        if (s[*i] == '|')
            add_spaces_s_c(s, n_s, i, j, '|');
        else if (s[*i] == '>' && s[*i + 1] == '>')
            add_spaces_d_c(s, n_s, i, j, '>');
        else if (s[*i] == '<' && s[*i + 1] == '<')
            add_spaces_d_c(s, n_s, i, j, '<');
        else if (s[*i] == '>')
            add_spaces_s_c(s, n_s, i, j, '>');
        else if (s[*i] == '<')
            add_spaces_s_c(s, n_s, i, j, '<');
        else
            n_s[(*j)++] = s[*i];
    }
    else
        n_s[(*j)++] = s[*i];
}

int count_special_characters(const char *str)
{
    int count = 0;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '|')
            count += 2;
        else if (str[i] == '>' || str[i] == '<')
        {
            if (str[i + 1] == str[i])
            {
                count += 2;
                i++;
            }
            count += 2;
        }
        i++;
    }

    return count;
}

// char *modify_str(char *str)
// {
//     int len;
//     int special_count;
//     int new_len;
//     int i;
//     int j;
//     int in_single_quote;
//     int in_double_quote;
//     char *new_str;

//     len = ft_strlen(str);
//     special_count = count_special_characters(str);
//     new_len = 2 * special_count + 2 * len;
//     new_str = malloc(new_len + 1 * sizeof(char));
//     if (!new_str)
//     {
//         perror("Failed to allocate memory");
//         exit(EXIT_FAILURE);
//     }
//     i = 0;
//     j = 0;
//     in_single_quote = 0;
//     in_double_quote = 0;
//     while (i < len && str[i] != '\0')
//     {
//         process_characters(str, new_str, &i, &j, &in_single_quote, &in_double_quote);
//         i++;
//     }
//     new_str[j] = '\0'; // Null-terminate the new string
//     return new_str;
// }

void init_vars(int *i, int *j, int *s_q, int *d_q)
{
    *i = 0;
    *j = 0;
    *s_q = 0;
    *d_q = 0;
}

void process_loop(char *s, char *ns, int l, int *i, int *j, int *sq, int *dq)
{
    while (*i < l && s[*i] != '\0')
    {
        process_characters(s, ns, i, j, sq, dq);
        (*i)++;
    }
}

char *modify_str(char *str)
{
    int len;
    int special_count;
    int new_len;
    char *new_str;
    int i;
    int j;
    int single_q;
    int double_q;

    len = ft_strlen(str);
    special_count = count_special_characters(str);
    new_len = 2 * special_count + 2 * len;
    new_str = malloc(new_len + 1 * sizeof(char));

    if (!new_str)
        exit(EXIT_FAILURE);

    init_vars(&i, &j, &single_q, &double_q);
    process_loop(str, new_str, len, &i, &j, &single_q, &double_q);
    new_str[j] = '\0';
    
    return new_str;
}
