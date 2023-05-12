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

	printf("\ni after read_font_directory == %zu\nfile.len == %zu\n", i, file.len);

	free(file.data); // TODO this was not freed in above error cases
}


static void print_table_directory(t_table_directory* tbl_dir, int tbl_size)
{
	printf("#)\ttag\tlen\toffset\n");
	for (int i = 0; i < tbl_size; ++i)
	{
		t_table_directory* t = tbl_dir + i;
		printf("%d)\t%c%c%c%c\t%d\t%d\n", i + 1,
			t->tag_c[3], t->tag_c[2],
			t->tag_c[1], t->tag_c[0],
			t->length, t->offset);
	}
}