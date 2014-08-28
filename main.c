#include <ctr/types.h>
#include <ctr/srv.h>
#include <ctr/APT.h>
#include <ctr/GSP.h>
#include <ctr/GX.h>
#include <ctr/GPU.h>
#include <ctr/HID.h>
#include <ctr/SHDR.h>
#include <ctr/svc.h>
#include "chip-8.h"
#include "utils.h"

extern void *PONG2_ch8;
extern int   PONG2_ch8_size;

u8  *gspHeap;
u32 *gxCmdBuf;
u8  currentBuffer;
u8  *topLeftFramebuffers[2];
Handle gspEvent, gspSharedMemHandle;
#define GET_CUR_BUF ((void*)&gspHeap[0x46500*currentBuffer])
#define SCREEN_TOP_W (400)
#define SCREEN_TOP_H (240)
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define BLACK  0x000000
#define WHITE  0xFFFFFF

void gspGpuInit();
void gspGpuExit();
void swapBuffers();
void copyBuffer();

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
    gspGpuInit();
    initSrv();
    aptInit(APPID_APPLICATION);
    gspGpuInit();
    hidInit(NULL);
    aptSetupEventHandler();
    
    struct chip8_context chip8;
    chip8_init(&chip8, 64, 32);
    chip8_loadrom_memory(&chip8, PONG2_ch8, PONG2_ch8_size);
    
    APP_STATUS status;
    while ((status = aptGetStatus()) != APP_EXITING) {
 
        if (status == APP_RUNNING) {
            u32 PAD = hidSharedMem[7];
            u32 regData=PAD|0x01000000;
            GSPGPU_WriteHWRegs(NULL, 0x202A04, (u32*)&regData, 4);

            chip8_step(&chip8);
            chip8_disp_to_buf(&chip8, GET_CUR_BUF, 10, 10);

            swapBuffers();
            copyBuffer();
            
        } else if (status == APP_SUSPENDING) {
            aptReturnToMenu();
        } else if (status == APP_SLEEPMODE) {
            aptWaitStatusEvent();
        }
        svc_sleepThread(16666666);
    }
    
    chip8_fini(&chip8);
    gspGpuExit();
    return 0;
}


void gspGpuInit()
{
	gspInit();

	GSPGPU_AcquireRight(NULL, 0x0);
	GSPGPU_SetLcdForceBlack(NULL, 0x0);

	//set subscreen to blue
	u32 regData=0x01FF0000;
	GSPGPU_WriteHWRegs(NULL, 0x202A04, &regData, 4);

	//grab main left screen framebuffer addresses
	GSPGPU_ReadHWRegs(NULL, 0x400468, (u32*)&topLeftFramebuffers, 8);

	//convert PA to VA (assuming FB in VRAM)
	topLeftFramebuffers[0]+=0x7000000;
	topLeftFramebuffers[1]+=0x7000000;

	//setup our gsp shared mem section
	u8 threadID;
	svc_createEvent(&gspEvent, 0x0);
	GSPGPU_RegisterInterruptRelayQueue(NULL, gspEvent, 0x1, &gspSharedMemHandle, &threadID);
	svc_mapMemoryBlock(gspSharedMemHandle, 0x10002000, 0x3, 0x10000000);

	//map GSP heap
	svc_controlMemory((u32*)&gspHeap, 0x0, 0x0, 0x2000000, 0x10003, 0x3);

	//wait until we can write stuff to it
	svc_waitSynchronization1(gspEvent, 0x55bcb0);

	//GSP shared mem : 0x2779F000
	gxCmdBuf=(u32*)(0x10002000+0x800+threadID*0x200);

	currentBuffer=0;
}

void gspGpuExit()
{
	GSPGPU_UnregisterInterruptRelayQueue(NULL);

	//unmap GSP shared mem
	svc_unmapMemoryBlock(gspSharedMemHandle, 0x10002000);
	svc_closeHandle(gspSharedMemHandle);
	svc_closeHandle(gspEvent);
	
	gspExit();

	//free GSP heap
	svc_controlMemory((u32*)&gspHeap, (u32)gspHeap, 0x0, 0x2000000, MEMOP_FREE, 0x0);
}

void swapBuffers()
{
	u32 regData;
	GSPGPU_ReadHWRegs(NULL, 0x400478, (u32*)&regData, 4);
	regData^=1;
	currentBuffer=regData&1;
	GSPGPU_WriteHWRegs(NULL, 0x400478, (u32*)&regData, 4);
}

void copyBuffer()
{
	//copy topleft FB
	u8 copiedBuffer=currentBuffer^1;
	u8* bufAdr=&gspHeap[0x46500*copiedBuffer];
	GSPGPU_FlushDataCache(NULL, bufAdr, 0x46500);

	GX_RequestDma(gxCmdBuf, (u32*)bufAdr, (u32*)topLeftFramebuffers[copiedBuffer], 0x46500);
}
