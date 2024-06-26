/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:29:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/13 12:02:23 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

void	p_handler(int fds[], char **av, char **env);

void	p_exec(char *path_av, char **env);

void	p_children(char **av, char **env, int fds[]);

void	close_fds(int fds[]);

void	p_adopted_children(char **av, char **env, int fds[]);

void	p_error_exit(int code, char *message);

char	*p_get_env(char **env);

char	*p_strjoin(char *first, char *second, char *add);

void	p_cleanup_array(char **arr);

char	*p_get_path(char *arg, char **env);

char	*p_helper_path(char *arg, char *path, char **paths, int i);

#endif