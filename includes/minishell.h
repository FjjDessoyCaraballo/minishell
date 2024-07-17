/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/17 13:33:43 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*************************************************/
/* User defined headers **************************/
/*************************************************/
# include "libft.h"
# include "../libft/includes/libft.h" // <- just to silence the nvim errors
# include "token.h"

/*************************************************/
/* external libraries ****************************/
/*************************************************/
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

/*************************************************/
/* macro *****************************************/
/*************************************************/
//Errors
# define ERR "Error\n"
# define EXIT "Exit\n"
# define ERR_ARG "Wrong number of arguments, Karen\n"
# define PATH_MAX 1024
# define SUCCESS 0
# define FAILURE 1

/*************************************************/
/* structs ***************************************/
/*************************************************/
typedef struct s_env
{
	char			*content;
	struct s_env	*next;
	struct s_env	*prev;
}			t_env;

typedef struct s_data
{
	char 	**env;
	int		exit_status;
	int		processes;
	int		*fd;
	char	*bin;
	char	*path;
	char	**binary_paths;
	char	*home_pwd;
	int		status;
	char	**cmd;
	// need to insert pids, tokens, and commands
	t_token *token;
	char	**builtins;
	char	**redirect;
	char	**cmd_a;
	char	*line_read;
	t_env	*envll;
}	t_data;


/*************************************************/
/* functions *************************************/
/*************************************************/

/* in main.c */
//usually I leave main.c alone. Its a style choice.


/* in execution.c */
int		execution(t_data *data, t_env **env_ll);
int		crack_pipe(t_data *data, t_token *token);
void	plumber_kindergarten(t_data *data, t_token *token);
int 	lonely_execution(t_data *data, t_token *token, t_env **env_ll);

/* in execution_utils.c */
int		err_pipes(char *msg, int err_code);
void	close_all_fds(int *fd);
int		*how_many_children(t_data *data, t_token *token);
char	*access_path(char **path, char *cmd);

/* in init.c */
void	ll_env(t_env **env_ll, char **env);
void	initializer(t_data *data, t_env **env_ll, char **env);
void	find_bin(t_env **env_ll, t_data *data);
char	*bin_extract(char *path);

/* in exit_handler.c */
void	error_exit(int num);

/* in line_handler.c */
int		sniff_line(t_data *data);

/* in ll_utils.c */
t_env	*ft_listnew(void *content);
char	*get_home(t_env *env_ll);
void	ft_listadd_back(t_env **lst, t_env *new);
t_env	*ft_list_last(t_env *lst);
void	free_ll(t_env *env_ll);

/* in ll_utils2.c */
void	env_arr_updater(t_data *data, t_env **env_ll);
int		ll_size(t_env **env_ll);

/* in built_ins.c */
int		built_ins(t_data *data, t_token *token, t_env **env_ll);
int		print_env(t_env *env_ll);
int		print_pwd(void);
void	get_the_hell_out(t_data *data, int exit_code, t_env *env_ll);
int		yodeling(char *echoes);

/* in built_ins2.c */
int		shell_cd(char *path, t_data *data);
int		export(char *cargo, t_env *env_ll);
int		print_export(t_env *env_ll);
int		unset(char *str, t_env **env_ll);

/* signals.c */
void	handler(int sig);

#endif
