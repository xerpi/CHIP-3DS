#include <3ds.h>
#include <time.h>
#include "chip-8.h"
#include "utils.h"

extern void *PONG2_bin;
extern int   PONG2_bin_size;

u8 *framebuf = NULL;
#define SCREEN_TOP_W (400)
#define SCREEN_TOP_H (240)
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define BLACK  0x000000
#define WHITE  0xFFFFFF


/*
void renderEffect()
{
	u8* bufAdr=&gspHeap[0x46500*currentBuffer];

	int i, j;
	for(i=1;i<400;i++)
	{
		for(j=1;j<240;j++)
		{
			u32 v=(j+i*240)*3;
			bufAdr[v]=(pcCos(i+cnt)+4096)/32;
			bufAdr[v+1]=(pcCos(j-256+cnt)+4096)/64;
			bufAdr[v+2]=(pcCos(i+128-cnt)+4096)/32;
		}
	}
	cnt++;
}
static inline void draw_plot_top(unsigned char *buffer, int x, int y, u32 color)
{
    unsigned char *p = (unsigned char *)(buffer + (SCREEN_TOP_H-y-1)*3 +x*3*SCREEN_TOP_H);
    p[0] = color & 0xFF;
    p[1] = (color>>8) & 0xFF;
    p[2] = (color>>16) & 0xFF;
}
*/
void chip8_disp_to_buf(struct chip8_context *ctx, unsigned char *buffer, int x_pos, int y_pos)
{
    int x, y;
    for (y = 0; y < ctx->disp_h; y++) {
        for (x = 0; x < ctx->disp_w; x++) {
            unsigned char *p = (unsigned char *)(buffer + (SCREEN_TOP_H-(y+y_pos)-1)*3 +(x+x_pos)*3*SCREEN_TOP_H);
            unsigned int color = ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? WHITE : BLACK;
            p[0] = color & 0xFF;       //B
            p[1] = (color>>8) & 0xFF;  //G
            p[2] = (color>>16) & 0xFF; //R
            //*(unsigned int*)(disp->pixels + y * disp->pitch + x*4) = //ARGB
              //  ((ctx->disp_mem[x/8 + (ctx->disp_w/8)*y]>>(7-x%8)) & 0b1) ? 0xFFFFFFFF : 0xFF000000;
        } 
    }
    
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	srvInit();
	aptInit();
	gfxInit();
	hidInit(NULL);
	aptSetupEventHandler();
    
    struct chip8_context chip8;
    chip8_init(&chip8, 64, 32);
    chip8_loadrom_memory(&chip8, PONG2_bin, PONG2_bin_size);
    
    APP_STATUS status;
    while ((status = aptGetStatus()) != APP_EXITING) {
        if (status == APP_RUNNING) {
			framebuf = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
			hidScanInput();
			
            chip8_step(&chip8);
            chip8_disp_to_buf(&chip8, framebuf, 10, 10);

			gfxFlushBuffers();
			gfxSwapBuffers();
            
        } else if (status == APP_SUSPENDING) {
            aptReturnToMenu();
        } else if (status == APP_SLEEPMODE) {
            aptWaitStatusEvent();
        }
        gspWaitForVBlank();
    }
    
    chip8_fini(&chip8);
	hidExit();
	gfxExit();
	aptExit();
	srvExit();
	return 0;
}
