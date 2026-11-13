/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/13 22:59:17 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	debug_print_opcode(t_data *data)
{
	printf("Opcode: 0x%X\n", data->opcode);
	printf("X: %d, Y: %d, N: %d, NN: %d, NNN: %d\n",
		data->opcode_x, data->opcode_y, data->opcode_n,
		data->opcode_nn, data->opcode_nnn);
	printf("PC: 0x%X, I: 0x%X\n", data->chip8.program_counter, data->chip8.index_register);
	printf("Registers: ");
	printf("[");
	for (int i = 0; i < 16; i++)
	{
		printf("0x%X", data->chip8.registers[i]);
		if (i < 15)
			printf(", ");
	}
	printf("]\n\n");
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

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_data		data;

	ft_bzero(&data, sizeof(t_data));
	init_chip8(&data.chip8);
	parse_arguments(argc, argv, &data);
	//ft_hex_dump(data.chip8.memory, RAM_SIZE / 4, 32);
	//ft_printf("\n\n");
	while (data.running)
	{
		//print_hex_bytes(&data.chip8.memory[START_ADDRESS + i++], 1);
		data.opcode = data.chip8.memory[data.chip8.program_counter++] << 8;
		data.opcode |= data.chip8.memory[data.chip8.program_counter++];
		decode(&data);
		execute(&data);
		if (data.chip8.program_counter >= START_ADDRESS + data.rom_size)
			data.running = false;
		//ft_hex_dump(&op_code, 2, 2);
		//ft_printf("%d\n", op_code);
		usleep(500000);
		debug_print_opcode(&data);
		print_display(data.chip8.display, DISPLAY_SIZE, 64);
	}
	//ft_printf("\n\n");
	//ft_hex_dump(data.chip8.memory + START_ADDRESS, data.rom_size, 2);
	return (EXIT_SUCCESS);
}
