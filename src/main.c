/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/11 21:15:10 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_get_path(char *arg, char **env)
{
	char	*path;
	char	**paths;
	char	*tmp;
	int		i;

	i = 0;
	paths = ft_split(p_get_env(env), ':');
	path = ft_strdup(arg);
	tmp = NULL;
	while (access(path, F_OK | X_OK) != 0 && paths[i])
	{
		if (path)
		{
			free(path);
			path = NULL;
		}
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, arg);
		free(tmp);
		i++;
	}
	p_cleanup_array(paths);
	if (access(path, F_OK | X_OK) != 0)
		p_error_exit(EXIT_FAILURE, "Command not found!\n");
	return (path);
}

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
	{
		dup2(fds[1], STDOUT_FILENO);
		dup2(in_fd, STDIN_FILENO);
		close(fds[0]);
		exec(av[1], env);
	}
	waitpid(pid, NULL, 0);
	dup2(fds[0], STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	close(fds[1]);
	exec(av[2], env);
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
