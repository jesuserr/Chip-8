/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/14 17:52:14 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

void	debug_print_opcode(t_data *data)
{
	printf("Opcode: 0x%X\n", data->opcode);
	printf("X: %d, Y: %d, N: %d, NN: %d, NNN: %d\n",
		data->opcode_x, data->opcode_y, data->opcode_n,
		data->opcode_nn, data->opcode_nnn);
	printf("PC: 0x%X, I: 0x%X\n", data->chip8.program_counter, \
		data->chip8.index_register);
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
	if (!init_window(&data))
	{
		destroy_window(&data);
		return (EXIT_FAILURE);
	}
	while (data.running)
	{
		process_input_events(&data);
		if (!data.emu_paused)
			instruction_cycle(&data);
		render_display(&data);
		SDL_Delay(data.cycle_delay);
	}
	destroy_window(&data);
	return (EXIT_SUCCESS);
}
