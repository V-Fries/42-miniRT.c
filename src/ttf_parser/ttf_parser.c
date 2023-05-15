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

static void print_table_directory(t_table_directory* tbl_dir, int tbl_size);
static void print_cmap(t_cmap* c);

void	ttf_parser(char *file_name)
{
	t_string				file;
	t_font_directory		font_directory;
	size_t					i;

	file = ft_read_file(file_name);
	if (file.data == NULL)
	{
		perror("ttf_parser failed");
		return ;
	}
	i = 0;
	if (read_font_directory(&file, &i, &font_directory) < 0)
	{
		printf("Failed to parse font\n"); // TODO better error
		return ;
	}
	print_table_directory(font_directory.table_directory, font_directory.offset_subtable.num_tables);

	const uint32_t	cmap_tag = read_uint32_unsafe("cmap");
	for(int j = 0; j < font_directory.offset_subtable.num_tables; ++j)
	{
		if (font_directory.table_directory[j].tag == cmap_tag)
		{
			t_cmap c = {0};
			if (read_cmap(&file, font_directory.table_directory[j].offset, &c) < 0)
				return (printf("Bad read_cmap()\n"), (void)0);
			print_cmap(&c);
//			(void)print_cmap;
			printf("\n%u\n", c.subtables->platform_specific_id);
			free(c.subtables);
		}
	}
//	t_cmap cmap;
//	if (read_cmap(&file, i, &cmap) < 0)
//	{
//		printf("Failed to parse font\n"); // TODO free
//		return ;
//	}
//	print_cmap(&cmap);

	printf("\ni after read_font_directory == %zu\nfile.len == %zu\n", i, file.len);

	free(file.data); // TODO this was not freed in above error cases
}


static void print_table_directory(t_table_directory* tbl_dir, int tbl_size)
{
	//TODO remove this function
	printf("#)\ttag\tlen\toffset\n");
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
