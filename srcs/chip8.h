/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chip8.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:51 by jesuserr          #+#    #+#             */
/*   Updated: 2026/10/25 20:50:25 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHIP8_H
# define CHIP8_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include <stdint.h>						// for fixed-width integer types
# include <stdio.h>							// for fread
# include <errno.h>							// for errno

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define RAM_SIZE				4096
# define START_ADDRESS			0x200
# define MAX_ROM_SIZE			(RAM_SIZE - START_ADDRESS)
# define FONTSET_START_ADDRESS	0x50
# define FONTSET_SIZE			80

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_chip8
{
	uint8_t		memory[RAM_SIZE];	// 4 KBytes RAM memory
	uint8_t		registers[16];		// 16 x 8-bit registers (V0 to VF)
	uint16_t	index_register;		// points to memory location
	uint16_t	program_counter;	// points to current instruction in memory
	uint16_t	stack[16];			// 16-level stack
	uint8_t		stack_pointer;		// points to the top of the stack
	uint8_t		delay_timer;		// counts down at 60 Hz when non-zero
	uint8_t		sound_timer;		// same as delay_timer, beeps when non-zero
	uint8_t		display[64 * 32];	// 64x32 monochrome display
	uint8_t		keypad[16];
}	t_chip8;

static const uint8_t	g_font_data[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
	0x20, 0x60, 0x20, 0x20, 0x70,	// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
	0xF0, 0x80, 0xF0, 0x80, 0x80};	// F

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/******************************************************************************/

#endif
