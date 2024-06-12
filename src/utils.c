/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:53:35 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 22:30:44 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	p_error_exit(int code, char *message)
{
	ft_fprintf(STDERR_FILENO, "%s\n", message);
	exit(code);
}

char	*p_get_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*p_strjoin(char *first, char *second, char *add)
{
	int		first_len;
	int		second_len;
	int		add_len;
	char	*new;

	add_len = ft_strlen(add);
	first_len = ft_strlen(first);
	second_len = ft_strlen(second);
	new = malloc(sizeof(char) * (first_len + add_len + second_len + 1));
	if (!new)
		return (NULL);
	ft_strcat(new, first);
	ft_strcat(new, add);
	ft_strcat(new, second);
	new[first_len + add_len + second_len + 1] = '\0';
	return (new);
}

void	p_cleanup_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*p_get_path(char *arg, char **env)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	if (p_get_env(env) == NULL)
		return (ft_strdup(arg));
	paths = ft_split(p_get_env(env), ':');
	if (!paths)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	path = ft_strdup(arg);
	if (!path)
	{
		p_cleanup_array(paths);
		p_error_exit(EXIT_FAILURE, strerror(errno));
	}
	while (access(path, F_OK | X_OK) != 0 && paths[i])
	{
		path = p_helper_path(arg, path, paths, i);
		i++;
	}
	p_cleanup_array(paths);
	return (path);
}
