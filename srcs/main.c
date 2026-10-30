/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/10/30 21:16:40 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	init_chip8(t_chip8 *chip8)
{
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

static void	parse_arguments(int argc, char **argv, t_data *data)
{
	FILE	*rom_file;

	if (argc < 2 || argc > 2)
	{
		ft_printf("Usage: %s <rom_file>\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	rom_file = fopen(argv[1], "rb");
	if (!rom_file)
	{
		perror("Error: Could not open ROM file.");
		exit (EXIT_FAILURE);
	}
	fseek(rom_file, 0, SEEK_END);
	data->rom_size = ftell(rom_file);
	if (data->rom_size > MAX_ROM_SIZE)
	{
		ft_printf("Error: ROM file is too large. ");
		ft_printf("Maximum size is %d bytes.\n", MAX_ROM_SIZE);
		fclose(rom_file);
		exit (EXIT_FAILURE);
	}
	rewind(rom_file);
	data->rom_size = fread(data->chip8.memory + START_ADDRESS, 1, \
	MAX_ROM_SIZE, rom_file);
	if (data->rom_size <= 0)
	{
		ft_printf("Error: ROM file is empty or could not be read.\n");
		fclose(rom_file);
		exit (EXIT_FAILURE);
	}
	fclose(rom_file);
	ft_printf("Successfully loaded ROM file: %s ", argv[1]);
	ft_printf("(%d bytes)\n", data->rom_size);
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	init_chip8(&data.chip8);
	parse_arguments(argc, argv, &data);
	ft_hex_dump(data.chip8.memory, RAM_SIZE / 4, 32);
}
