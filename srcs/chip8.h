/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chip8.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:30:51 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/22 21:49:39 by jesuserr         ###   ########.fr       */
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
# include <stdbool.h>						// for bool type
# include <SDL2/SDL.h>						// for SDL library
# include <SDL2/SDL2_gfxPrimitives.h>		// for SDL2_gfx graphics primitives
# include <time.h>							// for seeding random number

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define RAM_SIZE				4096
# define START_ADDRESS			0x200	// Start address for programs in memory
# define MAX_ROM_SIZE			(RAM_SIZE - START_ADDRESS)
# define STACK_SIZE				16
# define FONTSET_START_ADDRESS	0x50	// Start address for fontset in memory
# define FONTSET_SIZE			80		// Size of the fontset in bytes
# define DISPLAY_WIDTH			64
# define DISPLAY_HEIGHT			32
# define DISPLAY_SIZE			(DISPLAY_WIDTH * DISPLAY_HEIGHT)
# define PIXEL_SIZE				16			// Size of each block in SDL window
# define RGBA_WHITE				0xFFFFFFFF	// White color in RGBA format
# define CYCLE_DELAY_MS			16

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
	uint16_t	stack[STACK_SIZE];	// 16-level stack
	uint8_t		stack_pointer;		// points to the top of the stack
	uint8_t		delay_timer;		// counts down at 60 Hz when non-zero
	uint8_t		sound_timer;		// same as delay_timer, beeps when non-zero
	uint8_t		display[DISPLAY_SIZE];	// 64x32 monochrome display
	uint8_t		keypad[16];
}	t_chip8;

typedef struct s_sdl_window
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
}	t_sdl_window;

typedef struct s_data
{
	size_t			rom_size;
	uint16_t		opcode;
	uint8_t			opcode_x;		// 2nd nibble
	uint8_t			opcode_y;		// 3rd nibble
	uint8_t			opcode_n;		// 4th nibble
	uint8_t			opcode_nn;		// 3rd and 4th nibbles (sometimes called kk)
	uint16_t		opcode_nnn;		// 2nd, 3rd and 4th nibbles
	bool			running;
	t_chip8			chip8;
	t_sdl_window	sdl;			// SDL window and renderer
	uint32_t		cycle_delay;	// Delay between game loop iterations in ms
	bool			emu_paused;		// flag to pause the game loop
}	t_data;

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
/********************************* cycle.c ************************************/
void	instruction_cycle(t_data *data);

/********************************* graphics.c *********************************/
bool	init_window(t_data *data);
void	process_input_events(t_data *data);
void	destroy_window(t_data *data);
void	render_display(t_data *data);

/******************************** opcodes.c ***********************************/
void	opcode_0(t_data *data);
void	opcode_1(t_data *data);
void	opcode_2(t_data *data);
void	opcode_3(t_data *data);
void	opcode_4(t_data *data);
void	opcode_5(t_data *data);
void	opcode_6(t_data *data);
void	opcode_7(t_data *data);
void	opcode_8(t_data *data);
void	opcode_9(t_data *data);
void	opcode_a(t_data *data);
void	opcode_b(t_data *data);
void	opcode_c(t_data *data);
void	opcode_d(t_data *data);
void	opcode_f(t_data *data);
void	opcode_tbd(t_data *data);

/****************************** opcodes_8xyn.c ********************************/
void	opcode_8xy0(t_data *data);
void	opcode_8xy1(t_data *data);
void	opcode_8xy2(t_data *data);
void	opcode_8xy3(t_data *data);
void	opcode_8xy4(t_data *data);
void	opcode_8xy5(t_data *data);
void	opcode_8xy6(t_data *data);
void	opcode_8xy7(t_data *data);
void	opcode_8xye(t_data *data);

/****************************** opcodes_fxnn.c ********************************/
void	opcode_fx33(t_data *data);
void	opcode_fxn5(t_data *data);
void	opcode_fx07(t_data *data);
void	opcode_fx18(t_data *data);
void	opcode_fx29(t_data *data);
void	opcode_fx1e(t_data *data);

/********************************* parser.c ***********************************/
void	parse_arguments(int argc, char **argv, t_data *data);

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION POINTERS
*/

// Function pointer type for CHIP-8 opcode handlers. Each handler receives the
// emulator state and returns no value. The opcode table can then dispatch an
// instruction by indexing it with the opcode, rather than using a large
// switch-case block. This keeps instruction selection compact, makes each
// handler independent, and makes adding or changing opcodes less error-prone.
typedef void			(*t_opcode_func)(t_data *data);

#endif
