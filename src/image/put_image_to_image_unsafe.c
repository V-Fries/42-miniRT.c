#include "threads.h"
#include "image.h"

static void		*put_image_to_image_unsafe_routine(void *routine_arg);
inline static void	draw_line(unsigned int *dst_cursor, \
						const unsigned int *src_cursor, \
						const unsigned int *src_cursor_limit);

void	put_image_to_image_unsafe(t_image *dst, const t_image *src,
			t_vector2i position)
{
	t_put_image_to_image_unsafe_routine_arg	arg;

	arg.x_offset = position.x;
	arg.current_dst_line = dst->address + position.y * dst->width;
	arg.current_src_line = src->address;
	arg.dst = dst;
	arg.src = src;
	start_threads(&arg, &put_image_to_image_unsafe_routine);
}

static void	*put_image_to_image_unsafe_routine(void *routine_arg)
{
	t_put_image_to_image_unsafe_routine_arg	*data;
	const unsigned int						*src_cursor;
	unsigned int							*dst_cursor;
	const unsigned int						*src_cursor_limit;

	data = get_routine_data(routine_arg);
	mutex_lock(routine_arg);
	while (data->current_src_line < data->src->limit)
	{
		src_cursor = data->current_src_line;
		dst_cursor = data->current_dst_line;
		data->current_src_line += data->src->width;
		data->current_dst_line += data->dst->width;
		mutex_unlock(routine_arg);
		src_cursor_limit = src_cursor + data->src->width;
		dst_cursor += data->x_offset;
		draw_line(dst_cursor, src_cursor, src_cursor_limit);
		mutex_lock(routine_arg);
	}
	mutex_unlock(routine_arg);
	return (NULL);
}

static void	draw_line(unsigned int *dst_cursor, const unsigned int *src_cursor,
				const unsigned int *src_cursor_limit)
{
	while (src_cursor < src_cursor_limit)
	{
		*dst_cursor = mix_colors(*src_cursor, *dst_cursor);
		src_cursor++;
		dst_cursor++;
	}
}
