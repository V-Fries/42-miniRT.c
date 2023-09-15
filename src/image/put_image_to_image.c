/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_image_to_image.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:40:01 by vfries            #+#    #+#             */
/*   Updated: 2023/07/30 18:40:13 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"
#include "math/vector.h"
#include "threads.h"

static void		*put_image_to_image_routine(void *routine_arg);
inline static void	draw_line(unsigned int *dst_line, \
						const unsigned int *src_line, \
						const unsigned int *dst_line_limit, \
						const unsigned int *src_line_limit);

void	put_image_to_image(register t_image *dst, const t_image *src,
			t_vector2i position)
{
	t_put_image_to_image_routine_arg	arg;

	if (position.y > 0)
	{
		arg.current_dst_line = dst->address + position.y * dst->width;
		arg.current_src_line = src->address;
	}
	else
	{
		arg.current_dst_line = dst->address;
		arg.current_src_line = src->address + (-position.y) * src->width;
	}
	if (position.x > 0)
	{
		arg.src_x_start = 0;
		arg.dst_x_start = position.x;
	}
	else
	{
		arg.src_x_start = -position.x;
		arg.dst_x_start = 0;
	}
	arg.dst = dst;
	arg.src = src;
	start_threads(&arg, &put_image_to_image_routine);
}

static void	*put_image_to_image_routine(void *routine_arg)
{
	t_put_image_to_image_routine_arg	*data;
	unsigned int						*dst_line;
	const unsigned int					*src_line;
	const unsigned int					*dst_line_limit;
	const unsigned int					*src_line_limit;

	data = get_routine_data(routine_arg);
	mutex_lock(routine_arg);
	while (data->current_dst_line < data->dst->limit
		&& data->current_src_line < data->src->limit)
	{
		dst_line = data->current_dst_line;
		src_line = data->current_src_line;
		data->current_dst_line += data->dst->width;
		data->current_src_line += data->src->width;
		mutex_unlock(routine_arg);
		dst_line_limit = dst_line + data->dst->width;
		src_line_limit = src_line + data->src->width;
		dst_line += data->dst_x_start;
		src_line += data->src_x_start;
		draw_line(dst_line, src_line, dst_line_limit, src_line_limit);
		mutex_lock(routine_arg);
	}
	mutex_unlock(routine_arg);
	return (NULL);
}

inline static void	draw_line(unsigned int *dst_line,
						const unsigned int *src_line,
						const unsigned int *dst_line_limit,
						const unsigned int *src_line_limit)
{
	while (dst_line < dst_line_limit && src_line < src_line_limit)
	{
		*dst_line = mix_colors(*src_line, *dst_line);
		dst_line++;
		src_line++;
	}
}

void	put_image_to_image_unsafe(t_image *destination,
			const t_image *source, t_vector2i position)
{
	const unsigned int		*source_end
		= source->address + source->height * source->line_length;
	const int				x_max = source->width + position.x;
	register unsigned int	*source_curr;
	register unsigned int	*dest_curr;
	register int			x;

	dest_curr = destination->address + position.y * destination->line_length;
	x = position.x;
	source_curr = source->address;
	while (source_curr < source_end)
	{
		dest_curr[x] = mix_colors(*source_curr++, dest_curr[x]);
		x++;
		if (x >= x_max)
		{
			x = position.x;
			dest_curr += destination->line_length;
		}
	}
}
