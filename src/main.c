/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 15:28:58 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_handle_prog_name(char *arg, char **env)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	paths = ft_split(env[i], ':');
	paths[0] = ft_strdup_free(paths[0]);
	path = ft_strdup(arg);
	printf("path: %s\n", path);
	i = 0;
	while (access(path, F_OK | X_OK) != 0 && paths[i])
	{
		if (path)
		{
			free(path);
			path = NULL;
		}
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin_free(path, arg, FIRST);
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
		return (NULL);
	return (path);
}

void	handle_child(char *raw_path, int fds[], int fd, char **env)
{
	char	**args;
	char	*exec;

	args = ft_split(raw_path, ' ');
	exec = p_handle_prog_name(args[0], env);
	if (!exec)
	{
		printf("error has been found\n");
		exit(1);
	}
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fds[0]);
	execve(exec, args, NULL);
}

void	handle_parent(char *raw_path, int fds[], int fd, char **env)
{
	char	**args;
	char	*exec;

	args = ft_split(raw_path, ' ');
	exec = p_handle_prog_name(args[0], env);
	if (!exec)
	{
		printf("error has been found\n");
		exit(1);
	}
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fds[1]);
	execve(exec, args, NULL);
}

void	handle_exec(int ac, char **av, char **env)
{
	int		pid;
	int	 	fds[2];
	int		fd;

	(void)ac;
	fd = open(av[0], O_RDONLY);
	if (pipe(fds) < 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		handle_child(av[1], fds, fd, env);
	}
	waitpid(pid, NULL, 0);
	int other_fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	handle_parent(av[2], fds, other_fd, env);
	exit(0);
}

int main(int ac, char **av, char **env)
{
	ac--;
	av++;
	if (ac == 0)
	{
		printf("wowzies hello\n");
		return (0);
	}
	if (av[1][0] == '\0')
	{
		printf("no exec passed\n");
		exit(1);
	}
	handle_exec(ac, av, env);
	// printf("arg count: %d\n", ac);
}
