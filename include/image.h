/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_image.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 10:45:43 by vfries            #+#    #+#             */
/*   Updated: 2023/05/06 20:05:19 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

# include <stddef.h>

# include "window.h"
# include "math/vector.h"

typedef struct s_image
{
	void			*data;
	unsigned int	*address;
	unsigned int	*limit;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				height;
	int				width;
	size_t			size;
}	t_image;

typedef struct s_draw_circle_routine_arg
{
	t_vector2f		circle_center;
	float			radius_squared;
	float			radius;
	unsigned int	current_line;
	unsigned int	max_current_line;
	unsigned int	x_start;
	unsigned int	x_max;
	t_image			*image;
	unsigned int	color;
}	t_draw_circle_routine_arg;

typedef struct s_change_image_color_routine_arg
{
	unsigned int	*current_line;
	unsigned int	*image_end;
	unsigned int	incrementer;
	unsigned int	color;
}	t_change_image_color_routine_arg;

typedef struct s_round_image_corners_routine_arg
{
	int		current_line;
	int		radius;
	t_image	*image;
}	t_round_image_corners_routine_arg;

typedef struct s_put_background_routine_arg
{
	unsigned int	current_line_index;
	t_image			*dst;
	const t_image	*src;
}	t_put_background_routine_arg;

typedef struct s_put_image_to_image_data
{
	unsigned int		*next_dst_line;
	const unsigned int	*next_src_line;
	int					dst_x_start;
	int					src_x_start;
}	t_put_image_to_image_data;

typedef struct s_put_image_to_image_unsafe_routine_arg
{
	unsigned int		x_offset;
	unsigned int		*current_dst_line;
	const unsigned int	*current_src_line;
	const t_image		*dst;
	const t_image		*src;
}	t_put_image_to_image_unsafe_routine_arg;

void			init_image(t_image *image, t_window *window, int width,
					int height);
void			init_image_from_xpm(t_image *image, t_window *window,
					char *xmp_file);
void			destroy_t_image(t_window *minirt_window, t_image *image);
void			put_pixel_on_image(t_image *image, int y, int x,
					unsigned int color);
unsigned int	get_image_pixel_color(const t_image *image, int y, int x);

void			change_image_color(t_image *image, unsigned int color);
void			put_image_to_image(t_image *destination, const t_image *source,
					t_vector2i position);
void			put_image_to_image_unsafe(register t_image *destination,
					const t_image *source, t_vector2i position);
void			put_background(t_image *destination, const t_image *source);

unsigned int	mix_colors(unsigned int added_color, unsigned int base_color);

void			image_draw_circle(t_image *image, t_vector2f circle_center,
					float radius, unsigned int color);
void			image_draw_plus(t_image *image, float thickness,
					float border, unsigned int color);
void			image_draw_minus(t_image *image, int thickness,
					int border, unsigned int color);
void			image_draw_check_mark(t_image *image, unsigned int color,
					float border);


void			image_draw_outline(t_image *image, int width,
					unsigned int color);
void			image_draw_top_outline(t_image *image, int width,
					unsigned int color);
void			image_draw_bottom_outline(t_image *image, int width,
					unsigned int color);
void			image_draw_left_outline(t_image *image, int width,
					unsigned int color);
void			image_draw_right_outline(t_image *image, int width,
					unsigned int color);

void			round_image_corners(t_image *image, int radius);
bool			is_in_top_left_corner(int x, int y, int radius);
bool			is_in_top_right_corner(int x, int y, int radius,
					t_image *image);
bool			is_in_bottom_left_corner(int x, int y, int radius,
					t_image *image);
bool			is_in_bottom_right_corner(int x, int y, int radius,
					t_image *image);

#endif //IMAGE_H
