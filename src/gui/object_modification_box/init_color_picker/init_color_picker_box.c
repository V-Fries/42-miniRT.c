#include <errno.h>
#include <math.h>

#include "mlx.h"

#include "struct/t_gui_box.h"
#include "init.h"

static void			color_picker_draw(t_gui_box *self, t_minirt *minirt,
						int x_offset, int y_offset);
static void			update_image(t_gui_box *self, t_minirt *minirt);
static unsigned int	get_darker_color(double x, double limit,
						t_color base_color);
static unsigned int	get_lighter_color(double x, double limit, double start,
						t_color base_color);

int	init_color_picker_box(t_minirt *minirt, t_gui_box *gui_box,
		t_gui_box *parent)
{
	*gui_box = create_t_gui_box(minirt, parent, \
		(t_vector2i){
			.x = 0,
			.y = 0}, \
		(t_size_int_2d){
			.width = parent->size.x,
			.height = parent->size.y / 2 - 4});
	if (errno == EINVAL)
		return (-1);
	if (init_image(&gui_box->on_hover_image,
				   &minirt->window, parent->size.x, parent->size.y / 2 - 4)
		< 0)
		return (-1); // TODO free previous image
	gui_box->draw = &color_picker_draw;
	return (0);
}
#if defined __linux__

static void	color_picker_draw(t_gui_box *self, t_engine *minirt,
				int x_offset, int y_offset)
{
	if (minirt->gui.color_picker_base_color_was_changed)
	{
		update_image(self, minirt);
		minirt->gui.color_picker_base_color_was_changed = false;
	}
	minirt->gui.draw_gui_image(&minirt->main_image, &self->image,
		(t_point_int_2d){\
			.x = self->position.x + x_offset, \
			.y = self->position.y + y_offset}
	);
	if (mouse_is_hovering_box(self, get_mouse_position(self, minirt,
				x_offset, y_offset)) == false)
		return ;
	add_hover_color_circle(self, minirt, x_offset, y_offset);
	minirt->gui.draw_gui_image(&minirt->main_image, &self->on_hover_image,
		(t_vector2i){\
			.x = self->position.x + x_offset, \
			.y = self->position.y + y_offset});
}
#elif defined __APPLE__

static void	color_picker_draw(t_gui_box *self, t_minirt *minirt,
				int x_offset, int y_offset)
{
	if (minirt->gui.color_picker_base_color_was_changed)
	{
		update_image(self, minirt);
		minirt->gui.color_picker_base_color_was_changed = false;
	}
	mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
		self->image.data, self->position.x + x_offset,
		self->position.y + y_offset);
	if (mouse_is_hovering_box(self, get_mouse_position(self, minirt,
				x_offset, y_offset)) == false)
		return ;
	add_hover_color_circle(self, minirt, x_offset, y_offset);
	mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
		self->on_hover_image.data, self->position.x + x_offset,
		self->position.y + y_offset);
}
#else
# error "Unsuported OS"
#endif

static void	update_image(t_gui_box *self, t_minirt *minirt)
{
	int	y;
	int	x;
	int	limit;

	y = -1;
	while (++y < self->image.y)
	{
		x = -1;
		limit = (int)round((double)self->image.x / 2);
		while (++x < limit)
			put_pixel_on_image(&self->image, y, x, get_darker_color(x, limit,
					minirt->gui.color_picker_base_color));
		x--;
		while (++x < self->image.x)
			put_pixel_on_image(&self->image, y, x, get_lighter_color(x,
																	 self->image.x, limit,
																	 minirt->gui.color_picker_base_color));
	}
	round_image_corners(&self->image, 10);
}

static unsigned int	get_darker_color(double x, double limit,
						t_color base_color)
{
	const t_color	color = {
		.x = (int)round((double)base_color.x * x / limit),
		.y = (int)round((double)base_color.y * x / limit),
		.z = (int)round((double)base_color.z * x / limit),
	};

	return (rgb_to_uint(color));
}

static unsigned int	get_lighter_color(double x, double limit, double start,
						t_color base_color)
{
	const t_color	color = {
		.x = (int)round((double)base_color.x
			+ (255.0 - (double)base_color.x) * (x - start) / (limit - start)),
		.y = (int)round((double)base_color.y
			+ (255.0 - (double)base_color.y) * (x - start) / (limit - start)),
		.z = (int)round((double)base_color.z
			+ (255.0 - (double)base_color.z) * (x - start) / (limit - start)),
	};

	return (rgb_to_uint(color));
}