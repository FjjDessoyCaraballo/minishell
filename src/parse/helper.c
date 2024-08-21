#include "../../includes/minishell.h"

/**
 * this function operates in the same fashion as strchr()
 * by returning a pointer to the token specified by the type
 * which is given as a second parameter.
 */
t_token *find_token(t_token *token, t_type type)
{
        t_token *head;

        head = token;
        while (head != NULL)
        {
                if (type == head->type)
                        return (head);
                head = head->next;
        }
        head = NULL;
        return (NULL);
}

/**
 * this function will search for type of token and return 0 (1) if
 * the command can be found within the token list. In case it does not
 * find the specified type the function returns 1 (0).
 */
int     search_token_type(t_token *token, t_type type)
{
        t_token *head;

        head = token;
        while (head != NULL)
        {
                if (head->type == type)
                        return (1);
                head = head->next;
        }
        return (0);
}
