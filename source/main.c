#include <3ds.h>
#include <time.h>
#include "chip-8.h"
#include "utils.h"
#include "tinyfont.h"
#include "PONG2_bin.h"

u8 *framebuf_top = NULL;
u8 *framebuf_bot = NULL;

void chip8_disp_to_buf(struct chip8_context *ctx, unsigned char *buffer, int x_pos, int y_pos)
{
    int x, y;
    for (y = 0; y < ctx->disp_h; y++) {
        for (x = 0; x < ctx->disp_w; x++) {
			//"Normal" coordinates
            //unsigned char *p = (unsigned char *)(buffer + (SCREEN_TOP_H-(y+y_pos)-1)*3 +(x+x_pos)*3*SCREEN_TOP_H);
			//3DS coordinates
			unsigned char *p = (unsigned char *)(buffer + (((ctx->disp_h-y)+y_pos) +(x+x_pos)*SCREEN_TOP_H)*3);
			
            unsigned int color = ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? GREEN : BLUE;
            p[0] = color & 0xFF;       //B
            p[1] = (color>>8) & 0xFF;  //G
            p[2] = (color>>16) & 0xFF; //R
        } 
    }
    
}

int main()
{
	srand(time(NULL));
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInit();

	struct chip8_context chip8;
	chip8_init(&chip8, 64, 32);
	chip8_loadrom_memory(&chip8, PONG2_bin, PONG2_bin_size);

	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		framebuf_top = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		framebuf_bot = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

		memset(framebuf_top, 0x00, 240*400*3);
		memset(framebuf_bot, 0x00, 240*320*3);
		tinyfont_draw_stringf(GFX_TOP, 10, SCREEN_TOP_H - 20, GREEN, "CHIP-3DS by xerpi");

		chip8_step(&chip8);
		chip8_core_dump(&chip8);
		chip8_disp_to_buf(&chip8, framebuf_top, SCREEN_TOP_W/2 - chip8.disp_w/2, SCREEN_TOP_H/2 - chip8.disp_h/2);

		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	chip8_fini(&chip8);
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}
