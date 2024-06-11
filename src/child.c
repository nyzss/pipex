/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:27:47 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 21:32:42 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_children(char **av, char **env, int fds[], int fd)
{
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fds[0]);
	p_exec(av[1], env);
}

void	p_adopted_children(char **av, char **env, int fds[], int fd)
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fds[1]);
	p_exec(av[2], env);
}
