/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/22 21:59:17 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

void	init_chip8(t_chip8 *chip8)
{
	ft_bzero(chip8, sizeof(t_chip8));
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
	srand(time(NULL));
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
