#include "image.h"
#include "threads.h"

static void	*change_image_color_routine(void *routine_arg);

void	change_image_color(t_image *image, unsigned int color)
{
	t_change_image_color_routine_arg	arg;

	arg.current_line = image->address;
	arg.image_end = image->address + image->height * image->width;
	arg.incrementer = image->width;
	arg.color = color;
	start_threads(&arg, &change_image_color_routine);
}

static void	*change_image_color_routine(void *routine_arg)
{
	unsigned int						*cursor;
	unsigned int						*cursor_limit;
	t_change_image_color_routine_arg	*data;

	data = get_routine_data(routine_arg);
	mutex_lock(routine_arg);
	while (data->current_line < data->image_end)
	{
		cursor = data->current_line;
		data->current_line += data->incrementer;
		mutex_unlock(routine_arg);
		cursor_limit = cursor + data->incrementer;
		while (cursor < cursor_limit)
			*cursor++ = data->color;
		mutex_lock(routine_arg);
	}
	mutex_unlock(routine_arg);
	return (NULL);
}
