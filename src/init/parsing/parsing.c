/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 03:18:51 by vfries            #+#    #+#             */
/*   Updated: 2023/04/21 03:18:52 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "struct/t_minirt.h"
#include "init.h"
#include "libft.h"

int	parse_scene(t_minirt *minirt, const char *start_up_scene)
{
	const int	fd = open(start_up_scene, O_RDONLY);
	char		***scene_content;

	if (fd == -1)
	{
		perror("Error\nFailed to open .rt file");
		return (-1);
	}
	scene_content = get_scene_content(fd);
	close(fd);
	if (scene_content == NULL)
	{
		perror("Error\nFailed to init scene_content from .rt file");
		return (-1);
	}
	if (parse_scene_content(minirt, scene_content))
	{
		ft_putstr_fd("Failed to parse scene_content\n", STDERR_FILENO);
		free_scene_content(scene_content);
		return (-1);
	}
	free_scene_content(scene_content);
	return (0);
}
