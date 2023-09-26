/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 03:19:09 by vfries            #+#    #+#             */
/*   Updated: 2023/09/26 16:14:17 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "engine.h"
#include "parsing.h"
#include "ray_tracer_gui_api.h"

int	parse_light(t_engine *engine, char **scene_content_line,
		t_rt_file_requirements *rt_file_requirements)
{
	t_vector3f	position;
	float		brightness;
	t_color		color;
	t_light		light;

	if (ft_split_len(scene_content_line) != 4)
		return (error("Error\nFailed to get light line\n"));
	if (get_position(scene_content_line[1], &position) < 0)
		return (error("Error\nFailed to get light position\n"));
	if (get_lighting_ratio(scene_content_line[2], &brightness) < 0)
		return (error("Error\nFailed to get light ratio\n"));
	if (get_color(scene_content_line[3], &color) < 0)
		return (error("Error\nFailed to get light albedo\n"));
	color = vector3f_divide(color, 255.f);
	light = light_create(position, color, brightness);
	if (add_light(engine, light) < 0)
		return (free_light(&light), -1);
	return (0);
	(void)rt_file_requirements;
}
