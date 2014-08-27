#include "utils.h"


void chip8_disp_to_SDL(struct chip8_context *ctx, SDL_Surface *disp)
{
    int x, y;
    for (y = 0; y < disp->clip_rect.h; y++) {
        for (x = 0; x < disp->clip_rect.w; x++) {
            *(unsigned int*)(disp->pixels + y * disp->pitch + x*4) = //ARGB
                ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? 0xFFFFFFFF : 0xFF000000;
        } 
    }
    
}

void print_bin(uint8_t n)
{
    int i;
    for (i = 0; i < 8; i++) {
        printf("%c", (n>>(7-i))&0b1 ? 'x' : 'o');
    }
}
