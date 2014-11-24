#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>
#include <stddef.h>
#include "chip-8.h"

#define SCREEN_TOP_W (400)
#define SCREEN_TOP_H (240)
#define SCREEN_BOT_W (320)
#define SCREEN_BOT_H (240)
#define RED	0xFF0000
#define GREEN	0x00FF00
#define BLUE	0x0000FF
#define BLACK	0x000000
#define WHITE	0xFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

int chip8_loadrom(struct chip8_context *ctx, char *path);
int chip8_loadrom_memory(struct chip8_context *ctx, const void *addr, unsigned int size);
void chip8_disp_to_buf(struct chip8_context *ctx, unsigned char *buffer);
void draw_plot(unsigned char *buf, int buf_w, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void blit_scale(gfxScreen_t screen, unsigned char *buffer, int x, int y, int w, int h, int scale);


#ifdef __cplusplus
}
#endif

#endif
