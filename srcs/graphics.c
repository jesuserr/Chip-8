/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 12:43:15 by jesuserr          #+#    #+#             */
/*   Updated: 2026/11/14 17:55:01 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip8.h"

static void	render_strings(t_data *data);

// Initializes SDL subsystems and creates a centered borderless window with
// renderer for graphical display. Window dimensions are calculated based on
// display size multiplied by scale factor. Returns true on success, false on
// any SDL error. Must be called before any SDL rendering operations.
bool	init_window(t_data *data)
{
	int	width, height;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return (ft_printf("SDL Init Error: %s\n", SDL_GetError()), false);
	width = (DISPLAY_WIDTH + 2) * PIXEL_SIZE;
	height = (DISPLAY_HEIGHT + 2) * PIXEL_SIZE;
	data->sdl.window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
	if (data->sdl.window == NULL)
		return (ft_printf("SDL Create Window Error: %s\n", SDL_GetError()), false);
	data->sdl.renderer = SDL_CreateRenderer(data->sdl.window, -1, 0);
	if (data->sdl.renderer == NULL)
		return (ft_printf("SDL Create Renderer Error: %s\n", SDL_GetError()), false);
	data->cycle_delay = CYCLE_DELAY_MS;
	return (true);
}

// Processes SDL input events: ESC quits when only creator is present, SPACE/P
// toggles pause.
void	process_input_events(t_data *data)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			data->running = false;
		else if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_p \
		|| event.key.keysym.sym == SDLK_SPACE))
			data->emu_paused = !data->emu_paused;
	}
}

// Cleans up SDL resources by destroying renderer and window, then quits SDL.
// Should be called during program cleanup to prevent memory leaks (even so, SDL
// library has several leaks at exit).
void	destroy_window(t_data *data)
{
	if (data->sdl.renderer)
		SDL_DestroyRenderer(data->sdl.renderer);
	if (data->sdl.window)
		SDL_DestroyWindow(data->sdl.window);
	SDL_Quit();
}

// Renders text overlays on the display. Uses SDL2_gfx stringColor for text
// rendering.
static void	render_strings(t_data *data)
{
	uint16_t	bottom_line;

	bottom_line = PIXEL_SIZE * (1 + DISPLAY_HEIGHT);
	stringColor(data->sdl.renderer, PIXEL_SIZE, PIXEL_SIZE / 2, \
		"Chip-8 Emulator ", RGBA_WHITE);
	if (data->emu_paused)
		stringColor(data->sdl.renderer, PIXEL_SIZE, bottom_line, \
			"Emulation paused", RGBA_WHITE);
	else
		stringColor(data->sdl.renderer, PIXEL_SIZE, bottom_line, \
			"'Space or P' to pause ", RGBA_WHITE);
}

// Renders the complete display by clearing screen, drawing all pixels in a grid
// and overlaying text information. Each pixel is scaled by PIXEL_SIZE with
// 1-pixel spacing. Calls render_strings and SDL_RenderPresent to update the
// screen buffer.
void	render_display(t_data *data)
{
	SDL_Rect	rect;
	uint8_t		pixel, x, y;

	SDL_SetRenderDrawColor(data->sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(data->sdl.renderer);
	rect.w = PIXEL_SIZE - 1;
	rect.h = PIXEL_SIZE - 1;
	for (y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (x = 0; x < DISPLAY_WIDTH; x++)
		{
			rect.x = PIXEL_SIZE * (x + 1);
			rect.y = PIXEL_SIZE * (y + 1);
			pixel = data->chip8.display[y * DISPLAY_WIDTH + x];
			if (pixel == 0x00)
				SDL_SetRenderDrawColor(data->sdl.renderer, 15, 15, 15, 255);
			else
				SDL_SetRenderDrawColor(data->sdl.renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(data->sdl.renderer, &rect);
		}
	}
	render_strings(data);
	SDL_RenderPresent(data->sdl.renderer);
}
