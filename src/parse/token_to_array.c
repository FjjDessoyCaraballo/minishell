#include "../../includes/minishell.h"

char **tokens_to_array(t_token *token_list) {
    // Step 1: Count number of tokens
    int count = 0;
    t_token *current = token_list;
    while (current != NULL) {
        if (current->type == COMMAND || current->type == FLAG)
            count++;
        current = current->next;
    }

    // Step 2: Allocate memory for char ** array
    char **array = (char **)malloc((count + 1) * sizeof(char *));
    if (array == NULL) {
        return NULL;
    }

    // Step 3: Copy command and flag token values to the array
    current = token_list;
    int index = 0;
    while (current != NULL) {
        if (current->type == COMMAND || current->type == FLAG) {
            array[index] = current->value;
            index++;
        }
        current = current->next;
    }
    array[index] = NULL; // Terminate the array with NULL

    return array;
}

