/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:42:40 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/22 15:07:03 by walnaimi         ###   ########.fr       */
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
/**
 * Processes a character in a string, handling quote states and adding 
 * spaces around certain characters.
 *
 * Parameters:
 *  @param s (char *): The original string.
 *  @param n_s (char *): The new string with added spaces.
 *  @param i (int *): The index of the current character in the string.
 *  @param j (int *): The index of the current character in the new string.
 *  @param s_q (int *): A flag indicating whether a single quote is active.
 *  @param d_q (int *): A flag indicating whether a double quote is active.
 *
 * Returns:
 *  None
 */
void process_chars(char *s, char *n_s, int *i, int *j, int *s_q, int *d_q)
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

/**
 * Counts the number of special characters in a given string.
 *
 * @param str The input string.
 *
 * @return The number of special characters in the input string.
 *
 * @throws None.
 */
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

/**
 * Initializes variables to zero.
 * 
 * @param i Pointer to an integer to be initialized.
 * @param j Pointer to an integer to be initialized.
 * @param s_q Pointer to an integer to be initialized.
 * @param d_q Pointer to an integer to be initialized.
 * 
 * @return None
 */
void init_vars(int *i, int *j, int *s_q, int *d_q)
{
    *i = 0;
    *j = 0;
    *s_q = 0;
    *d_q = 0;
}

/**
 * Iterates over the input string `s` and processes special characters
 * until the end of the string is reached.
 * 
 * @param s The input string to process.
 * @param ns The new string to store the processed characters.
 * @param l The length of the input string.
 * @param i A pointer to the current index in the input string.
 * @param j A pointer to the current index in the new string.
 * @param sq A pointer to the flag indicating if we are inside a single quote.
 * @param dq A pointer to the flag indicating if we are inside a double quote.
 * 
 * @return None
 */
void process_loop(char *s, char *ns, int l, int *i, int *j, int *sq, int *dq)
{
    while (*i < l && s[*i] != '\0')
    {
        process_chars(s, ns, i, j, sq, dq);
        (*i)++;
    }
}

/**
 * Modifies the input string by processing special characters and returns
 * the modified string.
 * 
 * @param str The input string to be modified.
 * 
 * @return A newly allocated string with the modifications applied.
 */
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
