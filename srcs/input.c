/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 12:43:15 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/24 23:25:21 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

/*
** SDL scancodes map each CHIP-8 keypad key to a physical keyboard position:
**  1 2 3 C     1 2 3 4
**  4 5 6 D     q w e r
**  7 8 9 E     a s d f
**  A 0 B F     z x c v
** Scancodes remain stable across keyboard layouts, so the bound physical keys
** keep working regardless of the characters produced by SDL_Keycode.
*/
static const SDL_Scancode	g_keymap[16] = {
	SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
	SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
	SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
	SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V};

// Processes SDL keyboard events: ESC quits the emulator, SPACE/P toggles pause,
// and mapped scancodes update the CHIP-8 keypad state.
void	process_input_events(t_data *data)
{
	SDL_Event		event;
	const uint8_t	*keys;
	uint8_t			i;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			data->running = false;
		else if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_p \
		|| event.key.keysym.sym == SDLK_SPACE))
			data->emu_paused = !data->emu_paused;
	}
	keys = SDL_GetKeyboardState(NULL);
	for (i = 0; i < 16; i++)
		data->chip8.keypad[i] = keys[g_keymap[i]];
}
