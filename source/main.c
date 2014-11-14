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

void blit_scale(gfxScreen_t screen, unsigned char *buffer, int x, int y, int w, int h, int scale)
{
	u16 fbwidth;
	unsigned char *framebuf1 = gfxGetFramebuffer(screen, GFX_LEFT, &fbwidth, NULL);
	int i, j;
	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			unsigned char *src_ptr = buffer + (i + j*w)*3;
			int i2, j2;
			for (i2 = 0; i2 < scale; i2++) {
				for (j2 = 0; j2 < scale; j2++) {
					draw_plot(framebuf1, fbwidth, x+i*scale + i2, y+(h-j-1)*scale + j2, src_ptr[0], src_ptr[1], src_ptr[2]);
				}
			}
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
	
	#define SCALE 6
	int pos_x = SCREEN_TOP_W/2 - (chip8.disp_w/2)*SCALE;
	int pos_y = SCREEN_TOP_H/2 - (chip8.disp_h/2)*SCALE;
	
	u32 keys_down = 0, keys_up = 0;
	
	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();
		keys_down = hidKeysDown();
		keys_up = hidKeysUp();

		framebuf_top = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		framebuf_bot = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

		memset(framebuf_top, 0x00, 240*400*3);
		memset(framebuf_bot, 0x00, 240*320*3);
		tinyfont_draw_stringf(GFX_TOP, 10, SCREEN_TOP_H - 14, WHITE, "CHIP-3DS by xerpi");
		
		
		if (keys_down & KEY_UP) {
			chip8_key_press(&chip8, 1);
		} else if (keys_up & KEY_UP) {
			chip8_key_release(&chip8, 1);
		}
		if (keys_down & KEY_DOWN) {
			chip8_key_press(&chip8, 4);
		} else if (keys_up & KEY_DOWN) {
			chip8_key_release(&chip8, 4);
		}
		
		if (keys_down & KEY_X) {
			chip8_key_press(&chip8, 0xC);
		} else if (keys_up & KEY_X) {
			chip8_key_release(&chip8, 0xC);
		}
		if (keys_down & KEY_B) {
			chip8_key_press(&chip8, 0xD);
		} else if (keys_up & KEY_B) {
			chip8_key_release(&chip8, 0xD);
		}
		
		chip8_step(&chip8);
		chip8_core_dump(&chip8);

		chip8_disp_to_buf(&chip8, disp_buf);
		blit_scale(GFX_TOP, disp_buf,
			pos_x,
			pos_y,
			chip8.disp_w,
			chip8.disp_h,
			SCALE);

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
