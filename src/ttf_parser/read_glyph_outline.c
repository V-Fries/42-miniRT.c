/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_outline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 20:46:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/22 20:46:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ttf.h"

static int	read_endPtsOfContours(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline);
static int	read_instructions(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline);
static int	read_flags(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline, uint16_t last_index);

int	read_glyph_outline(const t_string *file, const t_ttf *ttf,
		uint16_t code_point, t_glyph_outline *outline)
{
	size_t			file_cursor;
	const int64_t	glyph_table_offset = ttf_get_table_offset(ttf, "glyf");
	uint16_t		last_index;

	if (glyph_table_offset < 0)
		return (-1);
	file_cursor = glyph_table_offset + get_glyph_offset(code_point, ttf);
//	file_cursor = glyph_table_offset + 0;
	*outline = (t_glyph_outline){0};
	if (read_uint16_move(file, &file_cursor, &outline->numberOfContours) < 0)
		return (-1);
	if (read_int16_move(file, &file_cursor, &outline->xMin) < 0)
		return (-1);
	if (read_int16_move(file, &file_cursor, &outline->yMin) < 0)
		return (-1);
	if (read_int16_move(file, &file_cursor, &outline->xMax) < 0)
		return (-1);
	if (read_int16_move(file, &file_cursor, &outline->yMax) < 0)
		return (-1);
	if (read_endPtsOfContours(file, &file_cursor, outline) < 0)
		return (-1);
	if (read_instructions(file, &file_cursor, outline) < 0)
		return (free(outline->endPtsOfContours), -1);
	last_index = outline->endPtsOfContours[outline->numberOfContours - 1];
	if (read_flags(file, &file_cursor, outline, last_index) < 0)
		return (free(outline->endPtsOfContours), free(outline->instructions),
			-1);
	if (read_x_coordinates(file, &file_cursor, outline, last_index) < 0)
		return (free(outline->endPtsOfContours), free(outline->instructions),
			free(outline->flags), -1);
	if (read_y_coordinates(file, &file_cursor, outline, last_index) < 0)
		return (free(outline->endPtsOfContours), free(outline->instructions),
			free(outline->flags), free(outline->xCoordinates), -1);
	return (0);
}

static int	read_endPtsOfContours(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline)
{
	size_t	i;

	outline->endPtsOfContours = malloc(outline->numberOfContours
			* sizeof(uint16_t));
	if (outline->endPtsOfContours == NULL)
		return (-1);
	i = -1;
	while (++i < outline->numberOfContours)
		if (read_uint16_move(file, file_cursor, outline->endPtsOfContours + i)
			< 0)
			return (free(outline->endPtsOfContours), -1);
	return (0);
}

static int	read_instructions(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline)
{
	uint16_t	i;

	if (read_uint16_move(file, file_cursor, &outline->instructionLength) < 0)
		return (-1);
	outline->instructions = malloc(outline->instructionLength);
	if (outline->instructions == NULL)
		return (-1);
	i = -1;
	while (++i < outline->instructionLength)
		if (read_uint8_move(file, file_cursor, outline->instructions + i) < 0)
			return (free(outline->instructions), -1);
	return (0);
}

static int	read_flags(const t_string *file, size_t *file_cursor,
				t_glyph_outline *outline, uint16_t last_index)
{
	const uint32_t	size = last_index + 1;
	size_t			i;
	uint8_t			repeat_count;

	outline->flags = malloc(size);
	if (outline->flags == NULL)
		return (-1);
	i = -1;
	while (++i < size)
	{
		if (read_uint8_move(file, file_cursor, &outline->flags[i].flag) < 0)
			return (free(outline->flags), -1);
		if (outline->flags[i].repeat)
		{
			if (read_uint8_move(file, file_cursor, &repeat_count) < 0)
				return (free(outline->flags), -1);
			while (repeat_count-- > 0)
			{
				i++;
				outline->flags[i] = outline->flags[i - 1];
			}
		}
	}
	return (0);
}
