/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 17:53:26 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_get_path(char *arg, char **env)
{
	char	*path;
	char	**paths;
	int		i;

	paths = ft_split(p_get_env(env), ':');
	if (!paths)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	paths[0] = ft_strdup_free(paths[0]);
	if (!paths[0])
		p_error_exit(EXIT_FAILURE, strerror(errno));
	path = ft_strdup(arg);
	if (!path)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	i = 0;
	while (access(path, F_OK | X_OK) != 0 && paths[i])
	{
		if (path)
		{
			free(path);
			path = NULL;
		}
		path = ft_strjoin(paths[i], "/");
		if (!path)
			p_error_exit(EXIT_FAILURE, strerror(errno));
		path = ft_strjoin_free(path, arg, FIRST);
		if (!path)
			p_error_exit(EXIT_FAILURE, strerror(errno));
		i++;
	}
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	if (access(path, F_OK | X_OK) != 0)
		p_error_exit(EXIT_FAILURE, "Command not found!\n");
	return (path);
}

void	handle_child(char *raw_path, int fds[], int fd, char **env)
{
	char	**args;
	char	*exec;
	int		i;

	i = 0;
	args = ft_split(raw_path, ' ');
	if (!args)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	exec = p_get_path(args[0], env);
	if (!exec)
	{
		printf("error has been found\n");
		exit(1);
	}
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fds[0]);
	if (execve(exec, args, NULL) < 0)
	{
		free(exec);
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
}

void	handle_parent(char *raw_path, int fds[], int fd, char **env)
{
	char	**args;
	char	*exec;
	int		i;

	i = 0;
	args = ft_split(raw_path, ' ');
	if (!args)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	exec = p_get_path(args[0], env);
	if (!exec)
	{
		printf("error has been found\n");
		exit(1);
	}
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fds[1]);
	if (execve(exec, args, NULL) < 0)
	{
		free(exec);
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
}

void	handle_exec(int fds[], char **av, char **env)
{
	int		pid;
	int		fd;
	int		status;

	fd = open(av[0], O_RDONLY);
	if (fd < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	pid = fork();
	if (pid < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	if (pid == 0)
		handle_child(av[1], fds, fd, env);
	if (waitpid(pid, &status, 0) < 0)
		p_error_exit(EXIT_FAILURE, "Error in waitpid\n");
	// if (WIFSIGNALED(status))
	// 	p_error_exit(EXIT_FAILURE, strerror(errno));
	int other_fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (other_fd < 0)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	handle_parent(av[2], fds, other_fd, env);
	exit(0);
}

int main(int ac, char **av, char **env)
{
	int	 	fds[2];

	ac--;
	av++;
	if (ac < 4)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		ft_printf("Try again...\n");
		return (2);
	}
	if (pipe(fds) < 0)
		p_error_exit(EXIT_FAILURE, "Couldn't create pipe!");
	handle_exec(fds, av, env);
}
