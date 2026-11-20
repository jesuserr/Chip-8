/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opcodes_fxnn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 20:38:53 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/20 22:50:29 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

// Stores the decimal digits of Vx in three consecutive bytes at address I.
void	opcode_fx33(t_data *data)
{
	uint8_t	value;

	ft_printf("Fx33 - LD B, Vx\n");
	if (data->chip8.index_register + 2 >= RAM_SIZE)
	{
		ft_printf("Memory overflow at PC=0x%X (Fx33)\n", \
			data->chip8.program_counter - 2);
		data->running = false;
		return ;
	}
	value = data->chip8.registers[data->opcode_x];
	data->chip8.memory[data->chip8.index_register] = value / 100;
	data->chip8.memory[data->chip8.index_register + 1] = (value / 10) % 10;
	data->chip8.memory[data->chip8.index_register + 2] = value % 10;
}

// Transfers registers V0 through Vx to or from memory starting at address I,
// or loads the delay timer from Vx for the Fx15 variant.
void	opcode_fxn5(t_data *data)
{
	uint8_t	x, i;

	x = data->opcode_x;
	if (data->opcode_nn == 0x55)
	{
		ft_printf("Fx55 - LD [I], Vx\n");
		if (data->chip8.index_register + x >= RAM_SIZE)
		{
			ft_printf("Memory overflow at PC=0x%X (Fx55)\n", \
				data->chip8.program_counter - 2);
			data->running = false;
			return ;
		}
		for (i = 0; i <= x; i++)
			data->chip8.memory[data->chip8.index_register + i] = \
				data->chip8.registers[i];
	}
	else if (data->opcode_nn == 0x65)
	{
		ft_printf("Fx65 - LD Vx, [I]\n");
		for (i = 0; i <= x; i++)
			data->chip8.registers[i] = \
				data->chip8.memory[data->chip8.index_register + i];
	}
	else if (data->opcode_nn == 0x15)
	{
		ft_printf("Fx15 - LD DT, Vx\n");
		data->chip8.delay_timer = data->chip8.registers[x];
	}
}

// Copies the current delay timer value into Vx.
void	opcode_fx07(t_data *data)
{
	ft_printf("Fx07 - LD Vx, DT\n");
	data->chip8.registers[data->opcode_x] = data->chip8.delay_timer;
}

// Copies the value of Vx into the sound timer.
void	opcode_fx18(t_data *data)
{
	ft_printf("Fx18 - LD ST, Vx\n");
	data->chip8.sound_timer = data->chip8.registers[data->opcode_x];
}

// Sets I to the font sprite address for the hexadecimal digit stored in Vx.
void	opcode_fx29(t_data *data)
{
	uint8_t	value;

	value = data->chip8.registers[data->opcode_x] & 0x0F;
	ft_printf("Fx29 - LD F, Vx\n");
	data->chip8.index_register = FONTSET_START_ADDRESS + value * 5;
}

// Wait for a key press, store the value of the key in Vx. All execution stops
// until a key is pressed, then the value of that key is stored in Vx.
void	opcode_fx0a(t_data *data)
{
	(void)data;
	ft_printf("TODO!!! Fx0A - LD Vx, K\n");
}

// Adds Vx to I and keeps the result within the 12-bit address range.
void	opcode_fx1e(t_data *data)
{
	ft_printf("Fx1E - ADD I, Vx\n");
	data->chip8.index_register += data->chip8.registers[data->opcode_x];
	data->chip8.index_register &= 0x0FFF;
}
