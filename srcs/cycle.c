/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 17:01:28 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/14 17:11:54 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	fetch(t_data *data);
static void	decode(t_data *data);
static void	execute(t_data *data);

static void	fetch(t_data *data)
{
	data->opcode = data->chip8.memory[data->chip8.program_counter++] << 8;
	data->opcode |= data->chip8.memory[data->chip8.program_counter++];
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
	switch (data->opcode >> 12)
	{
		case 0x0:
			opcode_0(data);
			break ;
		case 0x1:
			opcode_1(data);
			break ;
		case 0x6:
			opcode_6(data);
			break ;
		case 0x7:
			opcode_7(data);
			break ;
		case 0xA:
			opcode_a(data);
			break ;
		case 0xD:
			opcode_d(data);
			break ;
		default:
			ft_printf("Unknown opcode: %x\n", data->opcode);
			break ;
	}
}

void	instruction_cycle(t_data *data)
{
	fetch(data);
	decode(data);
	execute(data);
	if (data->chip8.program_counter >= START_ADDRESS + data->rom_size)
		data->running = false;
}
