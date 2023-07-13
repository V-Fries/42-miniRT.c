/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_float_input_box.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 17:55:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/02 17:55:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gui/box.h"
#include "gui/utils.h"
#include "gui/UI.h"

static void	init_left_box_image(t_image *image);
static void	init_center_box_image(t_image *image);
static void	init_right_box_image(t_image *image);

int	create_float_input_box(t_engine *engine, t_gui_box *gui_box,
		t_float_input_box_on_click on_click)
{
	if (create_horizontal_boxes(engine, gui_box, "20 60 20", 0) < 0)
		return (-1);
	gui_box->children.data[0].on_click = on_click.minus;
	gui_box->children.data[1].on_click = on_click.text_box;
	gui_box->children.data[2].on_click = on_click.plus;
	gui_box->children.data[0].draw = &default_gui_box_draw;
	gui_box->children.data[1].draw = &default_gui_box_draw;
	gui_box->children.data[2].draw = &default_gui_box_draw;
	init_left_box_image(&gui_box->children.data[0].image);
	init_center_box_image(&gui_box->children.data[1].image);
	init_right_box_image(&gui_box->children.data[2].image);
	return (0);
}

static void	init_left_box_image(t_image *image)
{
	int				x;
	size_t			y;
	const size_t	last_line_index = (image->height - 1) * image->width;
	const int		last_x = image->width - 1;

	change_image_color(image, COLOR_TRANSPARENT);
	x = -1;
	while (++x < image->width)
	{
//		image->address[x] = COLOR_BLACK;
//		image->address[image->width + x] = COLOR_BLACK;
		image->address[last_line_index + x] = COLOR_BLACK;
		image->address[last_line_index - image->width + x] = COLOR_BLACK;
	}
	y = 0;
	while (y < image->size)
	{
		image->address[y] = COLOR_BLACK;
		image->address[y + 1] = COLOR_BLACK;
		image->address[y + last_x] = COLOR_BLACK;
//		image->address[y + last_x - 1] = COLOR_BLACK;
		y += image->width;
	}
}

static void	init_center_box_image(t_image *image)
{
	int				x;
	size_t			y;
	const size_t	last_line_index = (image->height - 1) * image->width;
	const int		last_x = image->width - 1;

	change_image_color(image, COLOR_TRANSPARENT);
	x = -1;
	while (++x < image->width)
	{
//		image->address[x] = COLOR_BLACK;
//		image->address[image->width + x] = COLOR_BLACK;
		image->address[last_line_index + x] = COLOR_BLACK;
		image->address[last_line_index - image->width + x] = COLOR_BLACK;
	}
	y = 0;
	while (y < image->size)
	{
		image->address[y] = COLOR_BLACK;
//		image->address[y + 1] = COLOR_BLACK;
		image->address[y + last_x] = COLOR_BLACK;
//		image->address[y + last_x - 1] = COLOR_BLACK;
		y += image->width;
	}
}

static void	init_right_box_image(t_image *image)
{
	int				x;
	size_t			y;
	const size_t	last_line_index = (image->height - 1) * image->width;
	const int		last_x = image->width - 1;

	change_image_color(image, COLOR_TRANSPARENT);
	x = -1;
	while (++x < image->width)
	{
//		image->address[x] = COLOR_BLACK;
//		image->address[image->width + x] = COLOR_BLACK;
		image->address[last_line_index + x] = COLOR_BLACK;
		image->address[last_line_index - image->width + x] = COLOR_BLACK;
	}
	y = 0;
	while (y < image->size)
	{
		image->address[y] = COLOR_BLACK;
//		image->address[y + 1] = COLOR_BLACK;
		image->address[y + last_x] = COLOR_BLACK;
		image->address[y + last_x - 1] = COLOR_BLACK;
		y += image->width;
	}
}
