#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include "chip-8.h"

void chip8_disp_to_SDL(struct chip8_context *ctx, SDL_Surface *disp);

#endif
