/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/10/30 20:38:12 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	init_chip8(t_chip8 *chip8)
{
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

static void	parse_arguments(int argc, char **argv, t_chip8 *chip8)
{
	FILE	*rom_file;
	size_t	rom_size;

	if (argc < 2 || argc > 2)
	{
		ft_printf("Usage: %s <rom_file>\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	rom_file = fopen(argv[1], "rb");
	if (!rom_file)
	{
		perror("Error: Could not open ROM file");
		exit (EXIT_FAILURE);
	}
	fseek(rom_file, 0, SEEK_END);
	rom_size = ftell(rom_file);
	if (rom_size > MAX_ROM_SIZE)
	{
		ft_printf("Error: ROM file is too large. Maximum size is %d bytes.\n", MAX_ROM_SIZE);
		fclose(rom_file);
		exit (EXIT_FAILURE);
	}
	rewind(rom_file);
	rom_size = fread(chip8->memory + START_ADDRESS, 1, MAX_ROM_SIZE, rom_file);
	if (rom_size <= 0)
	{
		ft_printf("Error: ROM file is empty or could not be read.\n");
		fclose(rom_file);
		exit (EXIT_FAILURE);
	}
	fclose(rom_file);
	printf("Loaded %zu bytes into memory.\n", rom_size);
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
