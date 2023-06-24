/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:04:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/19 14:04:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdint.h>

#include "image.h"

#define INSIDE 0 // 0000
#define LEFT 1   // 0001
#define RIGHT 2  // 0010
#define BOTTOM 4 // 0100
#define TOP 8    // 1000

static uint8_t		get_region(t_vector2i point, t_image *img);
static t_vector2i	clip_line(t_vector2i p1, t_vector2i p2,
						uint8_t tested_region, t_image *img);

// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
bool	line_clipping(t_vector2i *start, t_vector2i *end, t_image *img)
{
	uint8_t	start_region;
	uint8_t	end_region;

	while (true)
	{
		start_region = get_region(*start, img);
		end_region = get_region(*end, img);
		if (!(start_region | end_region))
			return (true);
		else if (start_region & end_region)
			return (false);
		else if (start_region > end_region)
			*start = clip_line(*start, *end, start_region, img);
		else
			*end = clip_line(*start, *end, end_region, img);
	}
}

static uint8_t	get_region(t_vector2i point, t_image *img)
{
	uint8_t	region;

	region = INSIDE;
	if (point.y >= img->height)
		region |= TOP;
	else if (point.y < 0)
		region |= BOTTOM;
	if (point.x >= img->width)
		region |= RIGHT;
	else if (point.x < 0)
		region |= LEFT;
	return (region);
}

static t_vector2i	clip_line(t_vector2i p1, t_vector2i p2,
						uint8_t tested_region, t_image *img)
{
	if (tested_region & TOP)
		return ((t_vector2i){
			.x = p1.x + (p2.x - p1.x)
				* (img->height - 1 - p1.y) / (p2.y - p1.y),
			.y = img->height - 1});
	if (tested_region & BOTTOM)
		return ((t_vector2i){
			.x = p1.x + (p2.x - p1.x) * (.0 - p1.y) / (p2.y - p1.y),
			.y = 0.0});
	if (tested_region & RIGHT)
		return ((t_vector2i){
			.x = img->width - 1,
			.y = p1.y + (p2.y - p1.y)
				* (img->width - 1 - p1.x) / (p2.x - p1.x)});
	return ((t_vector2i){
		.x = 0.0,
		.y = p1.y + (p2.y - p1.y) * (.0 - p1.x) / (p2.x - p1.x)});
}
