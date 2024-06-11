/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 21:19:11 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec(char *path_av, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(path_av, ' ');
	path = p_get_path(args[0], env);
	if (execve(path, args, NULL) < 0)
	{
		free(path);
		p_cleanup_array(args);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
}

void	p_children(char **av, char **env, int fds[], int fd)
{
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fds[0]);
	exec(av[1], env);
}

void	p_parent(char **av, char **env, int fds[], int fd)
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fds[1]);
	exec(av[2], env);
}

void	p_execute(char **av, char **env)
{
	int	fds[2];
	int	pid;
	int	in_fd;
	int	out_fd;

	if (pipe(fds) < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	in_fd = open(av[0], O_RDONLY);
	out_fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	pid = fork();
	if (pid < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid == 0)
		p_children(av, env, fds, in_fd);
	waitpid(pid, NULL, 0);
	p_parent(av, env, fds, out_fd);
}

int main(int ac, char **av, char **env)
{
	ac--;
	av++;
	if (ac < 4)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		ft_printf("Try again...\n");
		return (2);
	}
	p_execute(av, env);
}
