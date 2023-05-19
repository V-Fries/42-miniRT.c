/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 03:26:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/12 03:26:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "ttf.h"

#include <stdio.h>

static void	print_table_directory(t_table_directory *tbl_dir, int tbl_size);
static void	print_cmap(t_cmap *c);
static void	print_format4(t_format4 *f4);

int	ttf_parser(t_ttf *ttf, char *file_name)
{
	t_string				file;

	// TODO make errors show file_name
	file = ft_read_file(file_name);
	if (file.data == NULL)
	{
		perror("ttf_parser failed");
		return (-1);
	}

	ft_bzero(ttf, sizeof(*ttf));

	if (read_font_directory(&file, &ttf->font_directory) < 0)
	{
		ft_print_error("Failed to parse font directory\n");
		free(file.data);
		return (-1);
	}
	print_table_directory(ttf->font_directory.table_directory,
		ttf->font_directory.offset_subtable.num_tables);

	if (read_cmap(&file, ttf) < 0)
		return (printf("Bad read_cmap()\n"), -1); // TODO free stuff
	print_cmap(&ttf->cmap);

	if (read_format4(&file, ttf) < 0)
	{
		printf("Failed to read format4\n");
		return (-1); // TODO free stuff
	}
	print_format4(ttf->format4);

	uint16_t c = 'A';
	while (ft_isalpha(c))
	{
		ft_printf("%c == %i\n", c, get_glyph_index(c, ttf->format4));
		c++;
	}


	(void)print_format4;
	free(file.data); // TODO this was not freed in above error cases
	return (0);
}


static void print_table_directory(t_table_directory* tbl_dir, int tbl_size)
{
	//TODO remove this function
	printf("\ntable_directory:\n#)\ttag\tlen\toffset\n");
	for (int i = 0; i < tbl_size; ++i)
	{
		t_table_directory* t = tbl_dir + i;
		printf("%d)\t%c%c%c%c\t%u\t%u\n", i + 1,
			t->tag_c[3], t->tag_c[2],
			t->tag_c[1], t->tag_c[0],
			t->length, t->offset);
	}
}

static void print_cmap(t_cmap* c)
{
	printf("\ncmap:\n#)\tpId\tpsID\toffset\ttype\n");
	for(int i = 0; i < c->number_subtables; ++i)
	{
		t_cmap_encoding_subtable* cet = c->subtables + i;
		printf("%d)\t%hu\t%hu\t%i\t", i+1, cet->platform_id, cet->platform_specific_id, cet->offset);
		switch(cet->platform_id)
		{
			case 0: printf("Unicode"); break;
			case 1: printf("Mac"); break;
			case 2: printf("Not Supported"); break;
			case 3: printf("Microsoft"); break;
		}
		printf("\n");
	}
}

static void print_format4(t_format4 *f4)
{
	printf("Format: %d, Length: %d, Language: %d, Segment Count: %d\n", f4->format, f4->length, f4->language, f4->segCountX2/2);
	printf("Search Params: (searchRange: %d, entrySelector: %d, rangeShift: %d)\n",
		   f4->searchRange, f4->entrySelector, f4->rangeShift);
	printf("Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset\n");
	for(int i = 0; i < f4->segCountX2/2; ++i) {
		printf("--------------:\t% 9d\t% 7d\t% 7d\t% 12d\n", f4->startCode[i], f4->endCode[i], f4->idDelta[i], f4->idRangeOffset[i]);
	}
}
