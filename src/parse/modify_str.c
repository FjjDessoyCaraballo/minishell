#include "../../includes/minishell.h"

void add_spaces_around_char(char *str, char *new_str, size_t *i, size_t *j, char ch)
{
    size_t len = strlen(str);

    // Add a space before the character if it's not the first character and the previous character is not a space
    if (*i > 0 && !isspace(str[*i - 1]))
        new_str[(*j)++] = ' ';
    new_str[(*j)++] = ch;

    // Add a space after the character if it's not the last character and the next character is not a space
    if (*i < len - 1 && !isspace(str[*i + 1]))
        new_str[(*j)++] = ' ';
}

void add_spaces_around_double_char(char *str, char *new_str, size_t *i, size_t *j, char ch)
{
    size_t len = strlen(str);

    // Add a space before the double character if it's not the first character and the previous character is not a space
    if (*i > 0 && !isspace(str[*i - 1]))
        new_str[(*j)++] = ' ';

    new_str[(*j)++] = ch;
    new_str[(*j)++] = ch;

    // Add a space after the double character if it's not the last character and the next character is not a space
    if (*i < len - 2 && !isspace(str[*i + 2]))
        new_str[(*j)++] = ' ';

    (*i)++; // Skip the next character
}

void process_characters(char *str, char *new_str, size_t *i, size_t *j, int *s_q, int *in_double_quote)
{
    if (str[*i] == '\'' && !(*in_double_quote))
        *s_q = !(*s_q); // Toggle single quote state
    else if (str[*i] == '"' && !(*s_q))
        *in_double_quote = !(*in_double_quote); // Toggle double quote state

    if (!(*s_q) && !(*in_double_quote))
    {
        if (str[*i] == '|')
            add_spaces_around_char(str, new_str, i, j, '|');
        else if (str[*i] == '>' && str[*i + 1] == '>')
            add_spaces_around_double_char(str, new_str, i, j, '>');
        else if (str[*i] == '<' && str[*i + 1] == '<')
            add_spaces_around_double_char(str, new_str, i, j, '<');
        else if (str[*i] == '>')
            add_spaces_around_char(str, new_str, i, j, '>');
        else if (str[*i] == '<')
            add_spaces_around_char(str, new_str, i, j, '<');
        else
            new_str[(*j)++] = str[*i];
    }
    else
    {
        new_str[(*j)++] = str[*i];
    }
}

size_t count_special_characters(const char *str)
{
    size_t count = 0;
    size_t i = 0;

    while (str[i])
    {
        if (str[i] == '|')
            count += 2;
        else if (str[i] == '>' || str[i] == '<')
        {
            if (str[i + 1] == str[i]) // Check for ">>" or "<<"
            {
                count += 2;
                i++; // Skip the next character as it's part of the special character
            }
            count += 2;
        }
        i++;
    }

    return count;
}

// char *modify_str(char *str)
// {
//     size_t len;
//     size_t special_count;
//     size_t new_len;
//     size_t i;
//     size_t j;
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

void init_vars(size_t *i, size_t *j, int *s_q, int *d_q)
{
    *i = 0;
    *j = 0;
    *s_q = 0;
    *d_q = 0;
}

void process_loop(char *s, char *ns, size_t l, size_t *i, size_t *j, int *sq, int *dq)
{
    while (*i < l && s[*i] != '\0')
    {
        process_characters(s, ns, i, j, sq, dq);
        (*i)++;
    }
}

char *modify_str(char *str)
{
    size_t len;
    size_t special_count;
    size_t new_len;
    char *new_str;
    size_t i;
    size_t j;
    int s_q;
    int d_q;

    len = ft_strlen(str);
    special_count = count_special_characters(str);
    new_len = 2 * special_count + 2 * len;
    new_str = malloc(new_len + 1 * sizeof(char));

    if (!new_str)
        exit(EXIT_FAILURE);

    init_vars(&i, &j, &s_q, &d_q);
    process_loop(str, new_str, len, &i, &j, &s_q, &d_q);
    new_str[j] = '\0';
    
    return new_str;
}
