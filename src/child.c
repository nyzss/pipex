/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:27:47 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 22:08:21 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_precheck_path(char *av)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (av[0] == '\0')
		p_error_exit(EXIT_FAILURE, "Path precheck failed");
	while (av[i])
	{
		if (av[i] != ' ')
			n++;
		i++;
	}
	if (n == 0)
		p_error_exit(EXIT_FAILURE, "Path precheck failed");
}

void	p_children(char **av, char **env, int fds[])
{
	int	fd;

	fd = open(av[0], O_RDONLY);
	if (fd < 0)
	{
		close(fds[0]);
		close(fds[1]);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fds[0]);
	close(fds[1]);
	close(fd);
	p_precheck_path(av[1]);
	p_exec(av[1], env, fds[1]);
}

void	p_adopted_children(char **av, char **env, int fds[])
{
	int	fd;

	fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		close(fds[0]);
		close(fds[1]);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fds[1]);
	close(fds[0]);
	close(fd);
	p_precheck_path(av[2]);
	p_exec(av[2], env, fds[0]);
}
