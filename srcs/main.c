/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/07 22:16:42 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

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
	}
	//ft_printf("\n\n");
	//ft_hex_dump(data.chip8.memory + START_ADDRESS, data.rom_size, 2);
	return (EXIT_SUCCESS);
}
