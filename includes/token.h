#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

typedef struct s_data t_data;
/*******************************************
 * enum assign types
 * 0. echo,cd,pwd...
 * 1. PATH=,ls, grep, chmod...
 * 2. $HOME, "Hello world"...
 * 3. |
 * 4. <,>,<<,>>
 * 5. first single quote in the command line
 * 6. last single quote in the command line
 * 7. first double quote in the command line
 * 8. last double quote in the command line
 */
typedef	enum e_type{
	BUILTIN = 0,
	COMMAND = 1,
	ARGUMENT = 2,
	PIPE = 3,
	REDIRECT = 4,
	F_SINGLE_Q = 5,
	L_SINGLE_Q = 6,
	F_DOUBLE_Q = 7,
	L_DOUBLE_Q = 8,
	UNKNOWN = 404,
}			t_type;

/*******************************************
 * type = takes from the enum above
 * value = shows the exact value of the type
 * id = index of the token
 * next = points to the next token
 */

typedef struct s_token{
	t_type	type;
	char	*value;
	int		id;
	struct s_token	*next;
}		t_token;

/*****************************************
 * in parse/tokenizer.c
 */
void	line_tokenization(t_data *data);

/*****************************************
 * in parse/ft_strtok.c
 */
char	*ft_strtok(char *str, const char *delim);
int		ft_charinstr(char c, const char *str);
int		ft_strcmp(char *s1, char *s2);

/*****************************************
 * in parse/init_token.c
 */
t_token* init_token();

/*****************************************
 * in parse/token_test.c
 */
void print_tokens(t_data *data);

# endif
