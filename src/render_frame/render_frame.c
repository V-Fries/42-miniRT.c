/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:12:50 by vfries            #+#    #+#             */
/*   Updated: 2023/05/07 18:38:22 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "mlx.h"
#include "libft.h"

#include "struct/t_minirt.h"
#include "struct/t_gui_box.h"
#include "render_frame.h"
#include "render.h"

static void	render_minirt(t_minirt *minirt);
void		draw(t_minirt *minirt);

int	render_frame(t_minirt *minirt)
{
	const struct timeval	start_time = get_current_time();

	render_minirt(minirt);
	print_fps_counter(minirt, start_time);
	return (0);
}

#if defined __linux__

static void	render_minirt(t_minirt *minirt)
{
	draw(minirt);
	if (minirt->gui.is_hidden && minirt->gui.hidden_ratio == 1.0)
		mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
								minirt->ray_traced_image.data, 0, 0);
	else
	{
		put_background(&minirt->main_image, &minirt->ray_traced_image);
		render_user_interface(minirt);
		mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
			minirt->main_image.data, 0, 0);
	}
}
#elif defined __APPLE__

static void	render_minirt(t_minirt *minirt)
{
	draw(minirt);
	mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
		minirt->ray_traced_image.data, 0, 0);
	render_user_interface(minirt);
}
#else
# error "Unsuported OS"
#endif

void	draw(t_minirt *minirt)
{
	t_image	*img_ptr = &minirt->ray_traced_image;

	for (int y = 0; y < img_ptr->height; y++)
	{
		for (int x = 0; x < img_ptr->width; x++)
		{
			t_ray ray = minirt->camera.rays[x + y * (int) minirt->camera.viewport.x];
			t_vector3 pixel_color = render_pixel(ray.origin, ray.direction);
			pixel_color = vector3_clamp(pixel_color, 0, 1);
			pixel_color = vector3_multiply(pixel_color, 255);
			put_pixel_on_image(img_ptr, img_ptr->height - y - 1, x, vec_rgb_to_uint(pixel_color));
		}
	}
}
