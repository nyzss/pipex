/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 21:30:56 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_exec(char *path_av, char **env)
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

void	close_pipe(int fds[])
{
	close(fds[0]);
	close(fds[1]);
}

void	p_handler(char **av, char **env)
{
	int	fds[2];
	int	pid;
	int	pid2;
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
	pid2 = fork();
	if (pid2 < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid2 == 0)
		p_adopted_children(av, env, fds, out_fd);
	close_pipe(fds);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
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
	p_handler(av, env);
	printf("this should show\n");
}
