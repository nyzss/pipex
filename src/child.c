/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:27:47 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 15:55:57 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_children(char **av, char **env, int fds[], int in_out_fd[])
{
	dup2(fds[1], STDOUT_FILENO);
	dup2(in_out_fd[0], STDIN_FILENO);
	close(fds[0]);
	close(in_out_fd[0]);
	close(in_out_fd[1]);
	p_exec(av[1], env);
}

void	p_adopted_children(char **av, char **env, int fds[], int in_out_fd[])
{
	dup2(fds[0], STDIN_FILENO);
	dup2(in_out_fd[1], STDOUT_FILENO);
	close(fds[1]);
	close(in_out_fd[0]);
	close(in_out_fd[1]);
	p_exec(av[2], env);
}
