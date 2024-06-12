/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 22:31:07 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_exec(char *path_av, char **env, int fd_to_close)
{
	char	**args;
	char	*path;

	args = ft_split(path_av, ' ');
	if (!args)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	path = p_get_path(args[0], env);
	if (access(path, F_OK | X_OK) != 0)
	{
		if (path)
			free(path);
		close(fd_to_close);
		p_cleanup_array(args);
		p_error_exit(EXIT_FAILURE, "Command not found!");
	}
	if (execve(path, args, NULL) < 0)
	{
		free(path);
		p_cleanup_array(args);
		close(fd_to_close);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
}

void	close_fds(int fds[])
{
	close(fds[0]);
	close(fds[1]);
}

void	p_handler(int fds[], char **av, char **env)
{
	int	pid;
	int	pid2;

	pid = fork();
	if (pid < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid == 0)
		p_children(av, env, fds);
	pid2 = fork();
	if (pid2 < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid2 == 0)
		p_adopted_children(av, env, fds);
	close_fds(fds);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int ac, char **av, char **env)
{
	int	fds[2];

	if (ac != 5)
		p_error_exit(EXIT_FAILURE, "Usage: ./pipex infile cmd1 cmd2 outfile");
	if (pipe(fds) < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	p_handler(fds, av + 1, env);
}

//close all fds, if first child cant execute still run second one
// if file cant open still continue
// check if arg is ""
// check environment if path doesnt exist