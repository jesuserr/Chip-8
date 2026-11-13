/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 22:57:32 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/13 23:00:48 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

// Casts (void *) to (unsigned char *) to access memory at byte level
void	print_display(const void *src, size_t len, size_t bytes_per_line)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			j;

	if (!src || (int)len < 1 || (int)bytes_per_line < 1)
		return ;
	i = 0;
	ptr = (unsigned char *)src;
	while (i < len)
	{
		j = 0;
		while (j++ < bytes_per_line)
		{
			if (*(ptr + i + j - 1) == 0x00)
				ft_printf(" ");
			else
				ft_printf("X");
		}
		ft_printf("\n%s", RESET);
		i += bytes_per_line;
	}
}
