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

typedef struct s_cmap_encoding_subtable
{
	uint16_t	platform_id;
	uint16_t	platform_specific_id;
	uint32_t	offset;
}	t_cmap_encoding_subtable;

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

void	ttf_parser(char *file_name);

int		read_font_directory(const t_string *file, size_t *i,
			t_font_directory *font_directory);

int		read_uint16(const t_string *file, size_t i, uint16_t *dest);
int		read_uint32(const t_string *file, size_t i, uint32_t *dest);
int		read_uint_16_move(const t_string *file, size_t *i, uint16_t *dest);
int		read_uint_32_move(const t_string *file, size_t *i, uint32_t *dest);

#endif
