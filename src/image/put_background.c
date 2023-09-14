#include "image.h"
#include "threads.h"

static void	*put_background_routine(void *routine_arg);

void	put_background(t_image *destination, const t_image *source)
{
	t_put_background_routine_arg	arg;

	arg.current_line_index = 0;
	arg.dst = destination;
	arg.src = source;
	start_threads(&arg, &put_background_routine);
}

static void	*put_background_routine(void *routine_arg)
{
	t_put_background_routine_arg	*data;
	unsigned int					line_index;
	unsigned int					*dst_cursor;
	unsigned int					*src_cursor;
	unsigned int					*limit;

	data = get_routine_data(routine_arg);
	mutex_lock(routine_arg);
	while (data->current_line_index < data->src->size)
	{
		line_index = data->current_line_index;
		data->current_line_index += data->dst->width;
		mutex_unlock(routine_arg);
		dst_cursor = data->dst->address + line_index;
		src_cursor = data->src->address + line_index;
		limit = dst_cursor + data->dst->width;
		while (dst_cursor < limit)
			*dst_cursor++ = *src_cursor++;
		mutex_lock(routine_arg);
	}
	mutex_unlock(routine_arg);
	return (NULL);
}
