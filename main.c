#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include <SDL2/SDL.h>
#include "chip-8.h"
#include "utils.h"

SDL_Surface *CreateSurface(Uint32 flags, int w, int h, const SDL_Surface* disp);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: ./CHIP-3DS rom\n");
        return -1;
    }

    srand(time(NULL));
    printf("Chip-3DS by xerpi\n");
    printf("Loading \"%s\"...", argv[1]);
    struct chip8_context chip8;
    chip8_init(&chip8, 64, 32);
    int ret = chip8_loadrom(&chip8, argv[1]);
    if (ret) printf("OK!\n");
    else {
        printf("ERROR.\n");
        chip8_fini(&chip8);
        return -1;
    }
    printf("Running \"%s\"...\n", argv[1]);
    
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
                    case SDLK_r:
                        chip8_reset(&chip8);
                        chip8_loadrom(&chip8, argv[1]);
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
        /*int x, y;
        for (y = 0; y < 32; y++) {
            for (x = 0; x < 8; x++) {
                int i;
                for (i = 0; i < 8; i++) {
                    printf("%c", (chip8.disp_mem[x+y*8]>>(7-i))&0b1 ? 'X' : 'O');
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\n\n");
        */
        
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
