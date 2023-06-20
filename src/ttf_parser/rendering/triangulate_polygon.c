/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulate_polygon.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 23:44:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/20 23:44:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font/render.h"

t_triangles	triangulate_polygon(t_list *polygon, float , float x_max)
{
	t_list	*cursor;

	cursor = polygon;
	while (cursor != NULL)
	{

		cursor = cursor->next;
	}
}
