/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:42:07 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_exec(char *path_av, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(path_av, ' ');
	if (!args)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	path = p_get_path(args[0], env);
	if (!path)
	{
		p_cleanup_array(args);
		p_error_exit(EXIT_FAILURE, "Command not found!\n");
	}
	if (execve(path, args, NULL) < 0)
	{
		free(path);
		p_cleanup_array(args);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
}

void	close_fds(int fds[], int in_fd, int out_fd)
{
	close(fds[0]);
	close(fds[1]);
	close(in_fd);
	close(out_fd);
}

void	p_handler(int fds[], char **av, char **env)
{
	int	pid;
	int	pid2;
	int	in_fd;
	int	out_fd;

	in_fd = open(av[0], O_RDONLY);
	if (in_fd < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	out_fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (in_fd < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	pid = fork();
	if (pid < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid == 0)
		p_children(av, env, fds, in_fd);
	pid2 = fork();
	if (pid2 < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid2 == 0)
		p_adopted_children(av, env, fds, out_fd);
	close_fds(fds, in_fd, out_fd);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int ac, char **av, char **env)
{
	int	fds[2];

	if (ac < 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		ft_printf("Try again...\n");
		return (2);
	}
	if (pipe(fds) < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	p_handler(fds, av + 1, env);
}
