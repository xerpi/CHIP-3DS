#include <string.h>
#include "utils.h"

int chip8_loadrom_file(struct chip8_context *ctx, const char *path)
{
	FS_archive archive = {
		ARCH_SDMC,
		(FS_path) {
			PATH_EMPTY,
			1,
			(u8 *)""
		}
	};
	FS_path fs_path = FS_makePath(PATH_CHAR, path);

	Handle fh;
	Result ret = FSUSER_OpenFileDirectly(NULL, &fh, archive, fs_path, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if (ret) return 0;

	FSFILE_Read(fh, NULL, 0, &(ctx->RAM[CHIP8_ROM_LOAD_ADDR]), CHIP8_ROM_MAX_SIZE);

	FSFILE_Close(fh);
	svcCloseHandle(fh);
	return 1;
}

int chip8_loadrom_memory(struct chip8_context *ctx, const void *addr, unsigned int size)
{
	memcpy(&(ctx->RAM[0x200]), addr, size);
	return size;
}

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

void draw_plot(unsigned char *buf, int buf_w, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	register unsigned char *p = buf + (y + x*buf_w)*3;
	p[0] = b;
	p[1] = g;
	p[2] = r;
}

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

