/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chip8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 21:42:28 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/16 20:46:53 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

void	init_chip8(t_chip8 *chip8)
{
	ft_bzero(chip8, sizeof(t_chip8));
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

// Reports opcodes that are not implemented by the emulator yet.
void	opcode_tbd(t_data *data)
{
	ft_printf("Opcode 0x%X not implemented yet.\n", data->opcode);
}

// Handles system instructions: CLS clears the display, while RET restores the
// program counter from the stack and stops execution if the stack is empty.
void	opcode_0(t_data *data)
{
	ft_printf("0nnn - SYS addr %x\n", data->opcode_nnn);
	if (data->opcode == 0x00E0)
	{
		ft_printf("00E0 - CLS\n");
		ft_bzero(data->chip8.display, sizeof(data->chip8.display));
	}
	else if (data->opcode == 0x00EE)
	{
		ft_printf("00EE - RET\n");
		if (data->chip8.stack_pointer == 0)
		{
			ft_printf("Stack underflow at PC=0x%X (RET)\n", \
				data->chip8.program_counter - 2);
			data->running = false;
			return ;
		}
		data->chip8.stack_pointer--;
		data->chip8.program_counter = \
			data->chip8.stack[data->chip8.stack_pointer];
	}
}

// Jumps to the address encoded in the lowest 12 bits of the opcode.
void	opcode_1(t_data *data)
{
	ft_printf("1nnn - JP addr\n");
	data->chip8.program_counter = data->opcode_nnn;
}

// Calls a subroutine by saving the current program counter on the stack.
void	opcode_2(t_data *data)
{
	ft_printf("2nnn - CALL addr\n");
	if (data->chip8.stack_pointer >= STACK_SIZE)
	{
		ft_printf("Stack overflow at PC=0x%X (CALL %x)\n", \
			data->chip8.program_counter - 2, data->opcode_nnn);
		data->running = false;
		return ;
	}
	data->chip8.stack[data->chip8.stack_pointer] = data->chip8.program_counter;
	data->chip8.stack_pointer++;
	data->chip8.program_counter = data->opcode_nnn;
}

// Skips the next instruction when Vx is equal to the immediate byte.
void	opcode_3(t_data *data)
{
	ft_printf("3xnn - SE Vx, byte\n");
	if (data->chip8.registers[data->opcode_x] == data->opcode_nn)
		data->chip8.program_counter += 2;
}

// Skips the next instruction when Vx differs from the immediate byte.
void	opcode_4(t_data *data)
{
	ft_printf("4xnn - SNE Vx, byte\n");
	if (data->chip8.registers[data->opcode_x] != data->opcode_nn)
		data->chip8.program_counter += 2;
}

// Skips the next instruction when Vx is equal to Vy.
void	opcode_5(t_data *data)
{
	ft_printf("5xy0 - SE Vx, Vy\n");
	if (data->chip8.registers[data->opcode_x] == \
		data->chip8.registers[data->opcode_y])
		data->chip8.program_counter += 2;
}

// Loads the immediate byte into register Vx.
void	opcode_6(t_data *data)
{
	ft_printf("6xnn - LD Vx, byte\n");
	data->chip8.registers[data->opcode_x] = data->opcode_nn;
}

// Adds the immediate byte to Vx; overflow wraps around an 8-bit register.
void	opcode_7(t_data *data)
{
	ft_printf("7xnn - ADD Vx, byte\n");
	data->chip8.registers[data->opcode_x] += data->opcode_nn;
}

// Skips the next instruction when Vx differs from Vy.
void	opcode_9(t_data *data)
{
	ft_printf("9xy0 - SNE Vx, Vy\n");
	if (data->chip8.registers[data->opcode_x] != \
		data->chip8.registers[data->opcode_y])
		data->chip8.program_counter += 2;
}

// Loads the address encoded in the lowest 12 bits of the opcode into I.
void	opcode_a(t_data *data)
{
	ft_printf("Annn - LD I, addr\n");
	data->chip8.index_register = data->opcode_nnn;
}

// Draws an n-byte sprite from memory[I] at (Vx, Vy), XORing set pixels onto
// the display, clipping at its edges, and setting VF on pixel collisions.
void	opcode_d(t_data *data)
{
	uint16_t	start;
	uint8_t		x0, y0, col, row, byte;

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
			if (byte & 0x80)
			{
				if (data->chip8.display[start + col])
					data->chip8.registers[0xF] = 1;
				data->chip8.display[start + col] ^= 0x01;
			}
			byte <<= 1;
		}
		start += DISPLAY_WIDTH;
	}
}
