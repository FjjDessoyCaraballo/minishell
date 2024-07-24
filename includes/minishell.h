/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/24 16:11:30 by fdessoy-         ###   ########.fr       */
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
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>

/*************************************************/
/* macro *****************************************/
/*************************************************/
//Errors
# define ERR "Error\n"
# define MALLOC "Malloc failure\n"
# define EXIT "Exit\n"
# define REDIRECT_OUT 2
# define REDIRECT_IN 1
# define NO_FILE 1
# define DIRECOTRY 69
# define FILE_PERMISSION_DENIED 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
// # define EXEC_NOT_FOUND -2
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
	int		nb_cmds;
	int		read_end;
	int		*fd;
	char	*bin;
	char	*path;
	char	**binary_paths;
	int		pipe_fd[2];
	int		fd_in;
	int		fd_out;
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

// int 	lonely_execution(t_data *data, t_token *token, t_env **env_ll);

/* in execution.c */
int		execution(t_data *data, t_env **env_ll);
int		multiple_cmds(t_data *data, t_token *token, t_env **env_ll);
void	piped_execution(t_data *data, t_env **envll, char *instruction, int child);
char	**parse_instruction(char *instruction, int redirect_flag);
char	*redirect_out(char **array, char *instruction, int flag, int index);

/* in execution2.c */
int		single_execution(t_data *data, t_token *token, t_env **env_ll);
void	single_child(t_data *data, t_token *token, t_env **env_ll);
int		single_parent(pid_t pid, int status);
void	filter_redirect(t_data *data, char *instruction, int child, char *file);

/* in execution_utils.c */
int		err_pipes(char *msg, int err_code);
void	close_all_fds(int *fd);
int		how_many_children(t_data *data, t_token *token);
char	*access_path(char **path, char *cmd);

/* in execution_utils2.c */
void 	dup_fds(t_data *data, int child, int fd_flag, char *file);
void	open_fdin(t_data *data, char *infile);
void	open_fdout(t_data *data, char *outfile);
void	close_fds(t_data *data);
void	exit_child(char *file, int err_code);

/* in execution_utils3.c */
char	**cl_to_array(t_token *token);
int		checking_access(t_data *data, char *instruction);
char	*get_binary(char *instruction);
char	*find_file(char *instruction, int redirect_flag);
char	*abs_path(char *command);

/* in init.c */
void	ll_env(t_env **env_ll, char **env);
void	initializer(t_data *data, t_env **env_ll, char **env);
void	find_bin(t_env **env_ll, t_data *data);
char	*bin_extract(char *path);

/* in exit_handler.c */
void	error_exit(int num);

/* in utils.c */
void	free_data(t_data *data, char *path, t_env **env, char **command_array);

/* in line_handler.c */
int		sniff_line(t_data *data);

/* in ll_utils.c */
t_env	*ft_listnew(void *content);
char	*get_home(t_env *env_ll);
void	ft_listadd_back(t_env **lst, t_env *new);
t_env	*ft_list_last(t_env *lst);
void	free_ll(t_env *env_ll);

/* in ll_utils2.c */
char	**env_arr_updater(t_env **env_ll);
int		ll_size(t_env **env_ll);

/* in built_ins.c */
int		built_ins(t_data *data, t_token *token, t_env **env_ll);
int		print_env(t_env *env_ll);
int		print_pwd(void);
void	get_the_hell_out(t_data *data, t_token *token, t_env *env_ll);
int		yodeling(t_token *token);

/* in built_ins2.c */
int		built_in_or_garbage(t_data *data, t_env **env_ll, t_token *token);
int		shell_cd(t_token *token, t_data *data);
int		export(t_token *token, t_env **env_ll);
int		print_export(t_env **env_ll);
int		unset(t_token *token, t_env **env_ll);

/* signals.c */
void	handler(int sig);

#endif
