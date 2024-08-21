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

void process_characters(char *str, char *new_str, size_t *i, size_t *j, int *in_single_quote, int *in_double_quote)
{
    if (str[*i] == '\'' && !(*in_double_quote))
        *in_single_quote = !(*in_single_quote); // Toggle single quote state
    else if (str[*i] == '"' && !(*in_single_quote))
        *in_double_quote = !(*in_double_quote); // Toggle double quote state

    if (!(*in_single_quote) && !(*in_double_quote))
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

char *modify_str(char *str)
{
    size_t len = strlen(str);
    size_t special_count = count_special_characters(str);
    size_t new_len = 2 * special_count + 2 * len; // Adjust length with special characters
    char *new_str = malloc(new_len + 1 * sizeof(char)); // +1 for null terminator
    if (!new_str)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    size_t i = 0;  // Index for original string
    size_t j = 0;  // Index for new_str
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (i < len && str[i] != '\0')
    {
        process_characters(str, new_str, &i, &j, &in_single_quote, &in_double_quote);
        i++;
    }
    new_str[j] = '\0'; // Null-terminate the new string

    return new_str;
}
