#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include <SDL2/SDL.h>
#include "chip-3ds.h"

void chip3ds_disp_to_SDL(struct chip3ds_context *ctx, SDL_Surface *disp);
SDL_Surface *CreateSurface(Uint32 flags, int w, int h, const SDL_Surface* disp);

int main()
{
    srand(time(NULL));
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Chip-3DS",
       SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED,
       640,
       320,
       SDL_WINDOW_SHOWN);
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Surface *chip8_disp = CreateSurface(SDL_SWSURFACE, 64, 32, screen);
    SDL_Event event;
    int running = 1;
    
    #define ROM "PONG"
    printf("Chip-3DS\n");
    printf("Loading '" ROM "'...");
    struct chip3ds_context chip3ds;
    chip3ds_init(&chip3ds, 64, 32);
    int ret = chip3ds_loadrom(&chip3ds, ROM);
    if (ret) printf("OK!\n");
    else {
        printf("ERROR.\n");
        chip3ds_fini(&chip3ds);
        return -1;
    }
    printf("Running '" ROM "'...\n");
    
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        break;
                    case SDLK_w:
                        chip3ds_key_press(&chip3ds, 7);
                        break;
                    case SDLK_s:
                        chip3ds_key_press(&chip3ds, 4);
                        break;
                }
                break;
            }
        }
        
        chip3ds_step(&chip3ds);
        //chip3ds_core_dump(&chip3ds);
        //usleep(16667);

        chip3ds_disp_to_SDL(&chip3ds, chip8_disp);
        SDL_BlitScaled(chip8_disp, NULL, screen, NULL);
        SDL_UpdateWindowSurface(window);
    }
    
    chip3ds_fini(&chip3ds);
    SDL_FreeSurface(chip8_disp);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void chip3ds_disp_to_SDL(struct chip3ds_context *ctx, SDL_Surface *disp)
{
    int x, y;
    for (y = 0; y < disp->clip_rect.h; y++) {
        for (x = 0; x < disp->clip_rect.w; x++) {
            *(unsigned int*)(disp->pixels + y * disp->pitch + x*4) = //ARGB
                ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? 0xFFFFFFFF : 0xFF000000;
        } 
    }
    
}


SDL_Surface *CreateSurface(Uint32 flags, int w, int h, const SDL_Surface* disp)
{

  SDL_PixelFormat fmt = *(disp->format);
  return SDL_CreateRGBSurface(flags, w, h,
                  fmt.BitsPerPixel,
                  fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
}
