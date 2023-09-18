/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_io.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:00:10 by vfries            #+#    #+#             */
/*   Updated: 2023/04/09 00:21:04 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IO_H
# define FT_IO_H

# include <unistd.h>

# include "ft_string.h"

char		*get_next_line_multi_fd(int fd);
char		*get_next_line(int fd);

char		**ft_get_files_in_directory(const char *path, const char *extension, bool ignore_hidden_files);

ssize_t		ft_print_error(const char *str);
void		ft_print_position_in_code(const char *file, const char *function,
				const int line);
ssize_t		ft_putchar_fd(const char c, int fd);
ssize_t		ft_putendl_fd(const char *s, int fd);
ssize_t		ft_putnbr_fd(int n, int fd);
ssize_t		ft_putstr_fd(const char *s, int fd);

// Bonus
ssize_t		ft_putstr(const char *s);
int			ft_printf(const char *format, ...);
t_string	ft_read_file(const char *file_name);

#endif
