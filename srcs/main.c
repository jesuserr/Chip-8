/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/10/25 19:31:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	init_chip8(t_chip8 *chip8)
{
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
	ft_memset(chip8->memory + START_ADDRESS, 255, 1);
}

static void	parse_arguments(int argc, char **argv, t_chip8 *chip8)
{
	(void)chip8;
	if (argc < 2 || argc > 2)
	{
		ft_printf("Usage: %s <rom_file>\n", argv[0]);
		exit (EXIT_FAILURE);
	}
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_chip8	chip8;

	ft_bzero(&chip8, sizeof(t_chip8));
	init_chip8(&chip8);
	parse_arguments(argc, argv, &chip8);
	ft_hex_dump(chip8.memory, RAM_SIZE / 4, 32);
}
