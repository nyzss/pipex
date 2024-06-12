/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 08:56:30 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 09:01:54 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*p_helper_path(char *arg, char *path, char **paths, int i)
{
	char	*tmp;

	tmp = NULL;
	if (path)
	{
		free(path);
		path = NULL;
	}
	tmp = ft_strjoin(paths[i], "/");
	if (!tmp)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	path = ft_strjoin(tmp, arg);
	if (!path)
		p_error_exit(EXIT_FAILURE, strerror(errno));
	free(tmp);
	return (path);
}
