/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chip8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 21:42:28 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/13 22:11:35 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

void	init_chip8(t_chip8 *chip8)
{
	ft_bzero(chip8, sizeof(t_chip8));
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

void	opcode_0(t_data *data)
{
	if (data->opcode == 0x00E0)
	{
		ft_printf("00E0 - CLS\n");
		ft_bzero(data->chip8.display, sizeof(data->chip8.display));
	}
}

void	opcode_1(t_data *data)
{
	ft_printf("1nnn - JP addr\n");
	data->chip8.program_counter = data->opcode_nnn;
}

void	opcode_6(t_data *data)
{
	ft_printf("6xnn - LD Vx, byte\n");
	data->chip8.registers[data->opcode_x] = data->opcode_nn;
}

void	opcode_7(t_data *data)
{
	ft_printf("7xnn - ADD Vx, byte\n");
	data->chip8.registers[data->opcode_x] += data->opcode_nn;
}

void	opcode_a(t_data *data)
{
	ft_printf("Annn - LD I, addr\n");
	data->chip8.index_register = data->opcode_nnn;
}

void	opcode_d(t_data *data)
{
	uint16_t	start;
	uint8_t		x0, y0, col, row, byte;
	bool		pixel;

	ft_printf("Dxyn - DRW Vx, Vy, nibble\n");
	x0 = data->chip8.registers[data->opcode_x] % DISPLAY_WIDTH;
	y0 = data->chip8.registers[data->opcode_y] % DISPLAY_HEIGHT;
	start = x0 + (y0 * DISPLAY_WIDTH);
	data->chip8.registers[0xF] = 0;
	for (row = 0; row < data->opcode_n; row++)
	{
		if ((y0 + row) >= DISPLAY_HEIGHT)
			break ;
		byte = data->chip8.memory[(data->chip8.index_register + row) & 0xFFF];
		for (col = 0; col < 8; col++)
		{
			if ((x0 + col) >= DISPLAY_WIDTH)
				break ;
			pixel = (data->chip8.display[start + col] & 0x01);
			data->chip8.display[start + col] ^= (byte & 0x80) >> 7;
			byte <<= 1;
			if (pixel && (data->chip8.display[start + col] & 0x01) == 0)
				data->chip8.registers[0xF] = 1;
		}
		start = start + DISPLAY_WIDTH;
	}
	ft_hex_dump(data->chip8.display, sizeof(data->chip8.display), 64);
}
