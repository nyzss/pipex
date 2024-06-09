/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:28:59 by okoca             #+#    #+#             */
/*   Updated: 2024/06/09 22:21:56 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_read_whole_file(char *path)
{
	int		fd;
	char	buf[1024];
	char	*content;
	int		bytes_read;
	int		total_size_file;

	total_size_file = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd,  buf, 1024);
		if (bytes_read < 0)
			return (NULL);
		total_size_file += bytes_read;
	}
	close(fd);
	content = (char *)malloc(sizeof(char) * (total_size_file + 1));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	bytes_read = read(fd, content, total_size_file);
	if (bytes_read < 0)
		return (NULL);
	close(fd);
	printf("total size: %d\n", total_size_file);
	return (content);
}

int main(int ac, char **av)
{
	char	*content;
	char	**prog_args;
	char	 *env_args[] = {"PATH=/bin", NULL};
	char	*path;

	ac--;
	av++;
	if (ac == 0)
	{
		printf("wowzies hello\n");
		return (0);
	}
	printf("args: %s\n", av[0]);
	content = p_read_whole_file(av[0]);
	prog_args = ft_split(av[1], ' ');
	if (access(prog_args[0], F_OK) == 0)
		execve(prog_args[0], &prog_args[1], env_args);
	else
	{
		path = ft_strjoin("/usr/bin/", prog_args[0]);
		execve(path, &prog_args[1], env_args);
	}
	// int i = 0;
	// while (prog_args[i])
	// {
	// 	printf("split args: %s\n", prog_args[i]);
	// 	i++;
	// }
	printf("split args: %s\n", prog_args[1]);
	printf("\ntext:\n%s\n", content);
}