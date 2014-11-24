#include <3ds.h>
#include <time.h>
#include "chip-8.h"
#include "utils.h"
#include "tinyfont.h"
#include "PONG2_bin.h"

u8 *framebuf_top = NULL;
u8 *framebuf_bot = NULL;

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

		if (keys_down & KEY_START) break;

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
		
		int i;
		for (i = 0; i < 10; i++) {
			chip8_step(&chip8);
		}
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
