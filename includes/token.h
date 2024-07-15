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
	struct s_token	*next;
	struct s_token  *prev;
}		t_token;

/*****************************************
 * in src/parse/tokenizer.c
 *****************************************/
void	line_tokenization(t_data *data);
int		chunky_checker(char *token, t_token *current_token, t_data *data);

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
int		ft_command_check(char *token, t_token *current_token, char *bin_paths);
int		ft_pipe_check(char *token, t_token *current_token);
int		ft_redirect_op_check(char *token, t_token *current_token, char **redirect);

int		ft_argument_check(char *token, t_token *current_token);

/*****************************************
 * in src/parse/token_test.c
 *****************************************/
void	print_tokens(t_data *data);
/*****************************************
 * in src/parse/check_utils.c
 *****************************************/
char *loop_path_for_binary(char *binary, char **paths);
char *ft_strndup(const char *s, size_t n);

# endif
