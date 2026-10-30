/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:41:11 by jesuserr          #+#    #+#             */
/*   Updated: 2026/10/30 22:11:56 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	load_rom(const char *rom_file_path, t_data *data)
{
	FILE	*rom_file;
	size_t	bytes_read;
	long	file_size;

	rom_file = fopen(rom_file_path, "rb");
	if (!rom_file)
	{
		perror("Error: Could not open ROM file");
		exit(EXIT_FAILURE);
	}
	fseek(rom_file, 0, SEEK_END);
	file_size = ftell(rom_file);
	if (file_size > MAX_ROM_SIZE)
	{
		ft_printf("Error: ROM file is too large. ");
		ft_printf("Maximum size is %d bytes.\n", MAX_ROM_SIZE);
		fclose(rom_file);
		exit(EXIT_FAILURE);
	}
	rewind(rom_file);
	bytes_read = fread(data->chip8.memory + START_ADDRESS, 1, \
	MAX_ROM_SIZE, rom_file);
	if (bytes_read == 0)
	{
		ft_printf("Error: ROM file is empty or could not be read.\n");
		fclose(rom_file);
		exit(EXIT_FAILURE);
	}
	data->rom_size = bytes_read;
	fclose(rom_file);
}

void	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		ft_printf("Usage: %s <rom_file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	load_rom(argv[1], data);
	ft_printf("Successfully loaded ROM file: %s ", argv[1]);
	ft_printf("(%d bytes)\n", (int)data->rom_size);
}
