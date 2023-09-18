/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:27:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/18 16:27:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

# include "engine.h"

//	draw.c
void	render_bounding_box(t_engine *engine);

//	draw_line.c
void	draw_line(t_raytraced_pixels *pixels,
				  t_vector2i point1, t_vector2i point2,
				  t_vector3f color);

//	objects_bvh_node.c
void	draw_objects_bvh_node(t_engine *engine, t_objects_bvh_node *node,
							  t_vector3f color);


#endif