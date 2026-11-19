/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opcodes_8xyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 20:38:53 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/19 22:25:17 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

// Copies the value of Vy into Vx.
void	opcode_8xy0(t_data *data)
{
	ft_printf("8xy0 - LD Vx, Vy\n");
	data->chip8.registers[data->opcode_x] = \
		data->chip8.registers[data->opcode_y];
}

// Performs a bitwise OR of Vx and Vy, storing the result in Vx.
void	opcode_8xy1(t_data *data)
{
	ft_printf("8xy1 - OR Vx, Vy\n");
	data->chip8.registers[data->opcode_x] |= \
		data->chip8.registers[data->opcode_y];
	data->chip8.registers[0xF] = 0;
}

// Performs a bitwise AND of Vx and Vy, storing the result in Vx.
void	opcode_8xy2(t_data *data)
{
	ft_printf("8xy2 - AND Vx, Vy\n");
	data->chip8.registers[data->opcode_x] &= \
		data->chip8.registers[data->opcode_y];
	data->chip8.registers[0xF] = 0;
}

// Performs a bitwise XOR of Vx and Vy, storing the result in Vx.
void	opcode_8xy3(t_data *data)
{
	ft_printf("8xy3 - XOR Vx, Vy\n");
	data->chip8.registers[data->opcode_x] ^= \
		data->chip8.registers[data->opcode_y];
	data->chip8.registers[0xF] = 0;
}

// Adds Vy to Vx, storing the 8-bit result in Vx and the carry in VF.
void	opcode_8xy4(t_data *data)
{
	uint16_t	sum;

	ft_printf("8xy4 - ADD Vx, Vy\n");
	sum = (uint16_t)data->chip8.registers[data->opcode_x] + \
		(uint16_t)data->chip8.registers[data->opcode_y];
	data->chip8.registers[data->opcode_x] = (uint8_t)(sum & 0x00FF);
	if (sum > 0x00FF)
		data->chip8.registers[0xF] = 1;
	else
		data->chip8.registers[0xF] = 0;
}

// Subtracts Vy from Vx, storing the result in Vx and setting VF if no borrow.
void	opcode_8xy5(t_data *data)
{
	uint8_t	vx, vy;

	ft_printf("8xy5 - SUB Vx, Vy\n");
	vx = data->chip8.registers[data->opcode_x];
	vy = data->chip8.registers[data->opcode_y];
	data->chip8.registers[data->opcode_x] = vx - vy;
	if (vx >= vy)
		data->chip8.registers[0xF] = 1;
	else
		data->chip8.registers[0xF] = 0;
}

// Shifts Vy right by one, storing the result in Vx and the shifted bit in VF.
void	opcode_8xy6(t_data *data)
{
	uint8_t	vy;

	ft_printf("8xy6 - SHR Vx {, Vy}\n");
	vy = data->chip8.registers[data->opcode_y];
	data->chip8.registers[data->opcode_x] = vy >> 1;
	if (vy & 0x01)
		data->chip8.registers[0xF] = 1;
	else
		data->chip8.registers[0xF] = 0;
}

// Subtracts Vx from Vy, storing the result in Vx and setting VF if no borrow.
void	opcode_8xy7(t_data *data)
{
	uint8_t	vx, vy;

	ft_printf("8xy7 - SUBN Vx, Vy\n");
	vx = data->chip8.registers[data->opcode_x];
	vy = data->chip8.registers[data->opcode_y];
	data->chip8.registers[data->opcode_x] = vy - vx;
	if (vy >= vx)
		data->chip8.registers[0xF] = 1;
	else
		data->chip8.registers[0xF] = 0;
}

// Shifts Vy left by one, storing the result in Vx and the shifted bit in VF.
void	opcode_8xye(t_data *data)
{
	uint8_t	vy;

	ft_printf("8xyE - SHL Vx {, Vy}\n");
	vy = data->chip8.registers[data->opcode_y];
	data->chip8.registers[data->opcode_x] = vy << 1;
	if (vy & 0x80)
		data->chip8.registers[0xF] = 1;
	else
		data->chip8.registers[0xF] = 0;
}
