/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 03:28:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/12 03:28:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#ifndef TTF_H
# define TTF_H

# include <stdint.h>

# include "libft.h"

typedef struct s_format4
{
	uint16_t	format;
	uint16_t	length;
	uint16_t	language;
	uint16_t	segCountX2;
	uint16_t	searchRange;
	uint16_t	entrySelector;
	uint16_t	rangeShift;
	uint16_t	*endCode;
	uint16_t	reservedPad;
	uint16_t	*startCode;
	uint16_t	*idDelta;
	uint16_t	*idRangeOffset;
	uint16_t	*glyphIdArray;
}	t_format4;

/// @struct s_offset_subtable
/// @var ::platform_id
/// identifies the platform for which the encoding subtable is intended
/// @var ::platform_specific_id
/// identifies the encoding subtable within the context of the platform
/// specified by platform_id
/// @var ::offset
/// gives the offset from the beginning of the cmap to the start of the encoding
/// subtable
typedef struct s_cmap_encoding_subtable
{
	uint16_t	platform_id;
	uint16_t	platform_specific_id;
	uint32_t	offset;
}	t_cmap_encoding_subtable;

/// @struct s_offset_subtable
/// @var ::version
/// specifies the version number of the cmap format
/// @var ::number_subtables
/// specifies the number of encoding subtables present in the cmap
/// @var ::subtables
/// array of t_cmap_encoding_subtable structures that represent the encoding
/// subtables in the cmap
typedef struct s_cmap
{
	uint16_t					version;
	uint16_t					number_subtables;
	t_cmap_encoding_subtable	*subtables;
}	t_cmap;

/// @struct s_offset_subtable
/// @var ::scaler_type
/// number that identifies the font type
/// @var ::num_tables
/// contains the number of tables excluding the table_directory and any
/// subtables
/// @var ::search_range
/// (maximum power of 2 &lt;= num_tables) * 16 ___
/// (16 represents the size of a directory entry)
/// @var ::entry_selector
/// log2(search_range / 16)
/// @var ::range_shift
/// num_tables * 16 - search_range
typedef struct s_offset_subtable
{
	uint32_t	scaler_type;
	uint16_t	num_tables;
	uint16_t	search_range;
	uint16_t	entry_selector;
	uint16_t	range_shift;
}	t_offset_subtable;

typedef struct s_table_directory
{
	union {
		char		tag_c[4];
		uint32_t	tag;
	};
	uint32_t	check_sum;
	uint32_t	offset;
	uint32_t	length;
}	t_table_directory;

typedef struct s_ttf_font_directory
{
	t_offset_subtable	offset_subtable;
	t_table_directory	*table_directory;
}	t_font_directory;


typedef struct s_ttf
{

}	t_ttf;

void		ttf_parser(char *file_name);

int			read_font_directory(const t_string *file, size_t *i,
				t_font_directory *font_directory);
int			read_cmap(const t_string *file, size_t i, t_cmap *cmap);
int			read_format4(const t_string *file, t_format4 **dest_format4,
				size_t i);
int			get_glyph_index(uint16_t code_point, t_format4 *format4);

int			read_uint16(const t_string *file, size_t i, uint16_t *dest);
uint16_t	read_uint16_unsafe(const char *str);
int			read_uint32(const t_string *file, size_t i, uint32_t *dest);
uint32_t	read_uint32_unsafe(const char *str);
int			read_uint16_move(const t_string *file, size_t *i, uint16_t *dest);
int			read_uint32_move(const t_string *file, size_t *i, uint32_t *dest);

#endif
