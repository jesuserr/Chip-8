/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/07 20:31:40 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	init_chip8(t_chip8 *chip8)
{
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_data		data;
	uint16_t	i;

	ft_bzero(&data, sizeof(t_data));
	init_chip8(&data.chip8);
	parse_arguments(argc, argv, &data);
	ft_hex_dump(data.chip8.memory, RAM_SIZE / 4, 32);
	ft_printf("\n\n");
	i = 0;
	while (i < data.rom_size)
		print_hex_bytes(&data.chip8.memory[START_ADDRESS + i++], 1);
	ft_printf("\n\n");
	ft_hex_dump(data.chip8.memory + START_ADDRESS, data.rom_size, 2);
	return (EXIT_SUCCESS);
}
