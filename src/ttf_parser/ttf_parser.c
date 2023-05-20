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

static void	print_table_directory(const t_table_directory *tbl_dir, int tbl_size);
static void	print_cmap(const t_cmap *c);
static void	print_format4(const t_format4 *f4);
static void print_head(const t_head *head);
static void print_maxp(t_maxp *maxp);

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
		return (ft_print_error("Bad read_cmap()\n"), -1); // TODO free stuff
	print_cmap(&ttf->cmap);

	if (read_format4(&file, ttf) < 0)
	{
		ft_print_error("Failed to read format4\n");
		return (-1); // TODO free stuff
	}
	print_format4(ttf->format4);
	uint16_t c = 'A';
	while (ft_isalpha(c))
	{
		ft_printf("%c == %i\n", c, get_glyph_index(c, ttf->format4));
		c++;
	}

	if (read_head(&file, ttf) < 0)
	{
		ft_print_error("Failed to read head\n");
		return (-1); // TODO free stuff
	}
	print_head(&ttf->head);

	if (read_maxp(&file, ttf) < 0)
	{
		ft_print_error("Failed to read maxp\n");
		return (-1); // TODO free stuff
	}
	print_maxp(&ttf->maxp);


	(void)print_format4;
	free(file.data); // TODO this was not freed in above error cases
	return (0);
}


static void print_table_directory(const t_table_directory* tbl_dir, int tbl_size)
{
	//TODO remove this function
	printf("\ntable_directory:\n#)\ttag\tlen\toffset\n");
	for (int i = 0; i < tbl_size; ++i)
	{
		const t_table_directory* t = tbl_dir + i;
		printf("%d)\t%c%c%c%c\t%u\t%u\n", i + 1,
			t->tag_c[3], t->tag_c[2],
			t->tag_c[1], t->tag_c[0],
			t->length, t->offset);
	}
}

static void print_cmap(const t_cmap* c)
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

static void print_format4(const t_format4 *f4)
{
	printf("Format: %d, Length: %d, Language: %d, Segment Count: %d\n", f4->format, f4->length, f4->language, f4->segCountX2/2);
	printf("Search Params: (searchRange: %d, entrySelector: %d, rangeShift: %d)\n",
		   f4->searchRange, f4->entrySelector, f4->rangeShift);
	printf("Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset\n");
	for(int i = 0; i < f4->segCountX2/2; ++i) {
		printf("--------------:\t% 9d\t% 7d\t% 7d\t% 12d\n", f4->startCode[i], f4->endCode[i], f4->idDelta[i], f4->idRangeOffset[i]);
	}
}

static void	t_fixed_print(const t_fixed f)
{
	printf("%f\n", (float)f /  65536.f);
}

static void	t_long_date_time_print(const t_long_date_time long_date_time)
{
	const time_t	unix_time = long_date_time - 2082844800;
	struct tm		*time_info;

	time_info = gmtime(&unix_time);
	printf("Date: %02d-%02d-%04d\n", time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
}

static void print_head(const t_head *head)
{
	printf("\nhead:\n");
	printf("\tversion: ");
	t_fixed_print(head->version);
	printf("\tfontRevision: ");
	t_fixed_print(head->fontRevision);
	printf("\tcheckSumAdjustment: %u\n", head->checkSumAdjustment);
	printf("\tmagicNumber: %u\n", head->magicNumber);
	printf("\tflags: %u\n", head->flags);
	printf("\tunitsPerEm: %u\n", head->unitsPerEm);
	printf("\tcreated: ");
	t_long_date_time_print(head->created);
	printf("\tmodified: ");
	t_long_date_time_print(head->modified);
	printf("\txMin: %d\n", head->xMin);
	printf("\tyMin: %d\n", head->yMin);
	printf("\txMax: %d\n", head->xMax);
	printf("\tyMax: %d\n", head->yMax);
	printf("\tmacStyle: %u\n", head->macStyle);
	printf("\tlowestRecPPEM: %u\n", head->lowestRecPPEM);
	printf("\tfontDirectionHint: %d\n", head->fontDirectionHint);
	printf("\tindexToLocFormat: %d\n", head->indexToLocFormat);
	printf("\tglyphDataFormat: %d\n", head->glyphDataFormat);
}

static void print_maxp(t_maxp *maxp)
{
	printf("\nmaxp:\n");
	printf("\tversion: ");
	t_fixed_print(maxp->version);
	printf("\tnumGlyphs: %u\n", maxp->numGlyphs);
	printf("\tmaxPoints: %u\n", maxp->maxPoints);
	printf("\tmaxContours: %u\n", maxp->maxContours);
	printf("\tmaxCompositePoints: %u\n", maxp->maxComponentPoints);
	printf("\tmaxCompositeContours: %u\n", maxp->maxComponentContours);
	printf("\tmaxZones: %u\n", maxp->maxZones);
	printf("\tmaxTwilightPoints: %u\n", maxp->maxTwilightPoints);
	printf("\tmaxStorage: %u\n", maxp->maxStorage);
	printf("\tmaxFunctionDefs: %u\n", maxp->maxFunctionDefs);
	printf("\tmaxInstructionDefs: %u\n", maxp->maxInstructionDefs);
	printf("\tmaxStackElements: %u\n", maxp->maxStackElements);
	printf("\tmaxSizeOfInstructions: %u\n", maxp->maxSizeOfInstructions);
	printf("\tmaxComponentElements: %u\n", maxp->maxComponentElements);
	printf("\tmaxComponentDepth: %u\n", maxp->maxComponentDepth);
}
