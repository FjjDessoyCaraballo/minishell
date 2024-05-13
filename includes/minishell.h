/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/05/13 15:12:20 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*************************************************/
/* libft *****************************************/
/*************************************************/
# include "libft.h"

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
/* error macro **********************************/
/*************************************************/
# define ERR "Error\n"
# define EXIT "Exit\n"
# define ERR_ARG "Wrong number of arguments, Karen\n"

/*************************************************/
/* structs ***************************************/
/*************************************************/
typedef struct s_env
{
	char			*ll;
	struct s_env	*next;
	struct s_env	*prev;
}			t_env;

typedef struct s_data
{
	char 	**env;
	int		exit_status;
	int		pipe;
	// need to insert pids, tokens, and commands
	char	*line_read;
	t_env	*envll;
}	t_data;

/*************************************************/
/* functions *************************************/
/*************************************************/

/* in main.c */


/* in init.c */
int		copy_env(char **env);
void	initializer(t_data *data, char **env);

/* in exit_handler.c */
void	error_exit(int num);

/* in line_handler.c */
void	sniff_line(t_data *data);

#endif