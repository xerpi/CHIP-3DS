#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include "chip-8.h"


#ifdef __cplusplus
extern "C" {
#endif

void chip8_disp_to_SDL(struct chip8_context *ctx, SDL_Surface *disp);


#ifdef __cplusplus
}
#endif

#endif
