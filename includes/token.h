#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"
# include <stdbool.h>
typedef struct s_data t_data;
typedef struct s_env t_env;
/*******************************************
 * enum assign types
 * 0. echo,cd,pwd...
 * 1. PATH=,ls, grep, chmod...
 * 2. $HOME, "Hello world"...
 * 3. |
 * 4. <,>,<<,>>
 * 5. flags, -n , -l, --version
 * 404. not found (default).
 ********************************************/

typedef	enum e_type{
	BUILTIN = 1,
	COMMAND = 2,
	ARGUMENT = 3,
	PIPE = 4,
	REDIRECT = 5,
	FLAG = 6,
	ENVVAR = 7,
	UNKNOWN = 404,
}			t_type;

/*******************************************
 * type = takes from the enum above
 * value = shows the exact value of the type
 * id = index of the token
 * next = points to the next token
 * prev = points to the previous token.
 *******************************************/

typedef struct s_token{
	t_type	type;
	char	*value;
	char	*path;
	int		id;
	bool	echoed;
	struct s_token	*next;
	struct s_token  *prev;
}		t_token;

/*****************************************
 * in src/parse/tokenizer.c
 *****************************************/
void	line_tokenization(t_data *data);
int		chunky_checker(char *token, t_token *current_token, t_data *data);
t_token	*find_token(t_token *token, t_type type);
int		search_token_type(t_token *token, t_type type);

/*****************************************
 * in src/parse/ft_strtok.c
 *****************************************/
char	*ft_strtok(char *str, const char *delim);
int		ft_charinstr(char c, const char *str);
int		ft_strcmp(char *s1, char *s2);

/*****************************************
 * in src/parse/init_token.c
 *****************************************/
t_token* init_token();

/*****************************************
 * in src/parse/chunky_check.c
 *****************************************/
int		ft_builtin_check(char *token, t_token *current_token, char **builtins);
int		ft_command_check(char *token, t_token *current_token, t_data *data);
int		ft_pipe_check(char *token, t_token *current_token);
int		ft_redirect_op_check(char *token, t_token *current_token, char **redirect);
int		ft_argument_check(char *token, t_token *current_token);

/*****************************************
 * in src/parse/token_test.c
 *****************************************/
void	print_tokens(t_data *data);
void	print_cmd(char **cmd_a);

/*****************************************
 * in src/parse/check_utils.c
 *****************************************/
char *loop_path_for_binary(char *binary, char **paths);
char *ft_strndup(const char *s, size_t n);

/*****************************************
 * in src/parse/freedom.c
 * ***************************************/
void free_my_boi(char **paths);

/******************************************
 * in src/parse/token_to_array.c
 * ****************************************/
char **tokens_to_array(t_token *token_list);

/*******************************************
 * in src/parse/ft-getenv.c
 * *****************************************/
char *ft_getenv(const char *token, t_env *env_ll);
# endif
