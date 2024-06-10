/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/10 14:55:03 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_handle_prog_name(char *arg, char **env)
{
	char	*path;
	int	i;

	i = 0;
	while (env[i])
	{
		printf("envs: %s\n", env[i]);
		i++;
	}

	if (access(arg, F_OK | X_OK) == 0)
		path = ft_strdup(arg);
	else
		path = ft_strjoin("/usr/bin/", arg);
	return (path);
}

void	handle_exec(int ac, char **av, char **env)
{
	char	**prog_args;
	int		pid;

	printf("count: %d\n", ac);
	printf("args: %s\n", av[0]);
	prog_args = ft_split(av[1], ' ');
	pid = fork();
	printf("pid: %d\n", pid);
	if (pid == 0)
	{
		execve(p_handle_prog_name(prog_args[0], env), prog_args, NULL);
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
	if (pipe(fd) < 0)
		return (1);
	tt(env);
	handle_exec(ac, av, env);
}
