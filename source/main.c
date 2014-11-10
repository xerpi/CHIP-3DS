#include <3ds.h>
#include <time.h>
#include "chip-8.h"
#include "utils.h"
#include "tinyfont.h"
#include "PONG2_bin.h"

u8 *framebuf_top = NULL;
u8 *framebuf_bot = NULL;

void chip8_disp_to_buf(struct chip8_context *ctx, unsigned char *buffer)
{
	int x, y;
	for (y = 0; y < ctx->disp_h; y++) {
		for (x = 0; x < ctx->disp_w; x++) {
			//"Normal" RGB texture
			unsigned char *p = (unsigned char *)(buffer + (x + y*ctx->disp_w)*3);
			//3DS texture
			//unsigned char *p = (unsigned char *)(buffer + ((ctx->disp_h-y) + x*ctx->disp_w)*3);
			
			unsigned int color = ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? GREEN : BLUE;
			p[0] = (color>>16) & 0xFF;	//R
			p[1] = (color>>8) & 0xFF;	//G
			p[2] = color & 0xFF;		//B
		} 
	}
	
}

#define draw_plot(buf,buf_w,x,y,r,g,b) \
	do { \
		buf[((y) + (x)*(buf_w))*3 + 0] = b; \
		buf[((y) + (x)*(buf_w))*3 + 1] = g; \
		buf[((y) + (x)*(buf_w))*3 + 2] = r; \
	} while (0)

void blit_scale_x2(gfxScreen_t screen, unsigned char *buffer, int x, int y, int w, int h)
{
	u16 fbwidth;
	unsigned char *framebuf1 = gfxGetFramebuffer(screen, GFX_LEFT, &fbwidth, NULL);
	int i, j;
	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			unsigned char r = buffer[(i + j*w)*3 + 0];
			unsigned char g = buffer[(i + j*w)*3 + 1];
			unsigned char b = buffer[(i + j*w)*3 + 2];
			draw_plot(framebuf1, fbwidth, x+i*2 + 0, y+(h-j-1)*2 + 0, r, g, b);
			draw_plot(framebuf1, fbwidth, x+i*2 + 1, y+(h-j-1)*2 + 0, r, g, b);
			draw_plot(framebuf1, fbwidth, x+i*2 + 0, y+(h-j-1)*2 + 1, r, g, b);
			draw_plot(framebuf1, fbwidth, x+i*2 + 1, y+(h-j-1)*2 + 1, r, g, b);
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
	unsigned char disp_buf[chip8.disp_w*chip8.disp_h*3];
	
	u32 keys = 0, old_keys = 0;
	
	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();
		keys = hidKeysDown();

		framebuf_top = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		framebuf_bot = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

		memset(framebuf_top, 0x00, 240*400*3);
		memset(framebuf_bot, 0x00, 240*320*3);
		tinyfont_draw_stringf(GFX_TOP, 10, SCREEN_TOP_H - 20, WHITE, "CHIP-3DS by xerpi");
		
		
		if ((keys & ~old_keys) & KEY_UP) {
			chip8_key_press(&chip8, 1);
		} else if ((~keys & old_keys) & KEY_UP) {
			chip8_key_release(&chip8, 1);
		}
		if ((keys & ~old_keys) & KEY_DOWN) {
			chip8_key_press(&chip8, 4);
		} else if ((~keys & old_keys) & KEY_DOWN) {
			chip8_key_release(&chip8, 4);
		}
		
		if ((keys & ~old_keys) & KEY_X) {
			chip8_key_press(&chip8, 0xC);
		} else if ((~keys & old_keys) & KEY_X) {
			chip8_key_release(&chip8, 0xC);
		}
		if ((keys & ~old_keys) & KEY_B) {
			chip8_key_press(&chip8, 0xD);
		} else if ((~keys & old_keys) & KEY_B) {
			chip8_key_release(&chip8, 0xD);
		}
		
		chip8_step(&chip8);
		chip8_core_dump(&chip8);
		
		chip8_disp_to_buf(&chip8, disp_buf);
		blit_scale_x2(GFX_TOP, disp_buf,
			SCREEN_TOP_W/2 - (chip8.disp_w/2)*2,
			SCREEN_TOP_H/2 - (chip8.disp_h/2)*2,
			chip8.disp_w,
			chip8.disp_h);

		gfxFlushBuffers();
		gfxSwapBuffers();
		old_keys = keys;
	}

	chip8_fini(&chip8);
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}
