#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include <SDL2/SDL.h>
#include "chip-8.h"
#include "utils.h"

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
    struct chip8_context chip8;
    chip8_init(&chip8, 64, 32);
    int ret = chip8_loadrom(&chip8, ROM);
    if (ret) printf("OK!\n");
    else {
        printf("ERROR.\n");
        chip8_fini(&chip8);
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
                        chip8_key_press(&chip8, 1);
                        break;
                    case SDLK_s:
                        chip8_key_press(&chip8, 4);
                        break;
                    case SDLK_UP:
                        chip8_key_press(&chip8, 0xC);
                        break;
                    case SDLK_DOWN:
                        chip8_key_press(&chip8, 0xD);
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        break;
                    case SDLK_w:
                        chip8_key_release(&chip8, 1);
                        break;
                    case SDLK_s:
                        chip8_key_release(&chip8, 4);
                        break;
                    case SDLK_UP:
                        chip8_key_release(&chip8, 0xC);
                        break;
                    case SDLK_DOWN:
                        chip8_key_release(&chip8, 0xD);
                        break;
                }
                break;
            }
        }
        
        chip8_step(&chip8);
        //chip8_core_dump(&chip8);
        //usleep(16667);

        chip8_disp_to_SDL(&chip8, chip8_disp);
        SDL_BlitScaled(chip8_disp, NULL, screen, NULL);
        SDL_UpdateWindowSurface(window);
    }
    
    chip8_fini(&chip8);
    SDL_FreeSurface(chip8_disp);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



SDL_Surface *CreateSurface(Uint32 flags, int w, int h, const SDL_Surface* disp)
{

  SDL_PixelFormat fmt = *(disp->format);
  return SDL_CreateRGBSurface(flags, w, h,
                  fmt.BitsPerPixel,
                  fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
}
