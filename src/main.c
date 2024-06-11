/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 09:16:10 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_handle_prog_name(char *arg, char **env)
{
	char	*path;
	int		i;
	char	**paths;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	paths = ft_split(env[i], ':');
	tmp = paths[0];
	paths[0] = ft_strdup(paths[0] + 5);
	free(tmp);
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
		printf("path: %s\n", path);
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

void	handle_exec(int ac, char **av, char **env)
{
	char	**prog_args;
	char	*path;
	int		pid;

	printf("count: %d\n", ac);
	printf("args: %s\n", av[0]);
	prog_args = ft_split(av[1], ' ');
	pid = fork();
	printf("pid: %d\n", pid);
	if (pid == 0)
	{
		path = p_handle_prog_name(prog_args[0], env);
		if (!path)
		{
			printf("error has been found\n");
			exit(1);
		}
		execve(path, prog_args, NULL);
	}
	waitpid(pid, NULL, 0);
	exit(0);
}

int main(int ac, char **av, char **env)
{
	int	fd[2];

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
	if (pipe(fd) < 0)
		return (1);
	handle_exec(ac, av, env);
	printf("arg count: %d\n", ac);
}
