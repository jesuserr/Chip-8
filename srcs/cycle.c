/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 17:01:28 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/22 21:55:49 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static bool	fetch(t_data *data);
static void	decode(t_data *data);
static void	execute(t_data *data);
static const t_opcode_func	g_opcodes[16] = {
	opcode_0, opcode_1, opcode_2, opcode_3, opcode_4, opcode_5,
	opcode_6, opcode_7, opcode_8, opcode_9, opcode_a, opcode_b,
	opcode_c, opcode_d, opcode_tbd, opcode_f};

static bool	fetch(t_data *data)
{
	if (data->chip8.program_counter > RAM_SIZE - 2)
	{
		ft_printf("Memory overflow at PC=0x%X\n", data->chip8.program_counter);
		data->running = false;
		return (false);
	}
	data->opcode = data->chip8.memory[data->chip8.program_counter++] << 8;
	data->opcode |= data->chip8.memory[data->chip8.program_counter++];
	return (true);
}

static void	decode(t_data *data)
{
	data->opcode_x = (data->opcode & 0x0F00) >> 8;
	data->opcode_y = (data->opcode & 0x00F0) >> 4;
	data->opcode_n = data->opcode & 0x000F;
	data->opcode_nn = data->opcode & 0x00FF;
	data->opcode_nnn = data->opcode & 0x0FFF;
}

static void	execute(t_data *data)
{
	g_opcodes[data->opcode >> 12](data);
}

void	instruction_cycle(t_data *data)
{
	if (!fetch(data))
		return ;
	decode(data);
	execute(data);
	if (data->chip8.program_counter >= START_ADDRESS + data->rom_size)
		data->running = false;
}
