/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chip8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 21:42:28 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/07 22:25:11 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

void	init_chip8(t_chip8 *chip8)
{
	chip8->program_counter = START_ADDRESS;
	ft_memcpy(chip8->memory + FONTSET_START_ADDRESS, g_font_data, FONTSET_SIZE);
}

void	opcode_0(t_data *data)
{
	if (data->opcode == 0x00E0)
		ft_printf("CLS\n");
	else if (data->opcode == 0x00EE)
		ft_printf("RET\n");
	else
		ft_printf("SYS addr: 0x%X\n", data->opcode_nnn);
}
