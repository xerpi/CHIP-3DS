/*
* Copyright (C) 2014, xerpi
*/


#include "tinyfont.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
extern const unsigned char msx_font[];


void tinyfont_draw_char(gfxScreen_t screen, int x, int y, unsigned int color, char c)
{
	u16 width;
	unsigned char *framebuf1 = gfxGetFramebuffer(screen, GFX_LEFT, &width, NULL);
	unsigned char *framebuf2 = gfxGetFramebuffer(screen, GFX_RIGHT, NULL, NULL);
	unsigned char *glyph = (unsigned char*)(msx_font + c * 8);
	int i, j;
	for (i = 0; i < 8; ++i, ++glyph) {
		for (j = 0; j < 8; ++j) {
			if ((*glyph & (128 >> j))) {
				u8 *p1 = (u8 *)(framebuf1 + ((y+(7-i)) +(x+j)*width)*3);
				u8 *p2 = (u8 *)(framebuf2 + ((y+(7-i)) +(x+j)*width)*3);
				p1[0] = p2[0] = color & 0xFF;		//B
				p1[1] = p2[1] = (color>>8) & 0xFF;	//G
				p1[2] = p2[2] = (color>>16) & 0xFF; //R
			}
		}
	}
}


void tinyfont_draw_string(gfxScreen_t screen, int x, int y, unsigned int color, const char *string)
{
	if(string == NULL) return;
	int startx = x;
	const char *s = string;
	while(*s) {
		if(*s == '\n') {
			x = startx;
			y+=8;
		} else if(*s == '\t') {
			x+=8*4;
		} else {
			tinyfont_draw_char(screen, x, y, color, *s);
			x+=8;
		}
		++s;
	}
}


void tinyfont_draw_stringf(gfxScreen_t screen, int x, int y, unsigned int color, const char *s, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, s);
	vsnprintf(buffer, 256, s, args);
	tinyfont_draw_string(screen, x, y, color, buffer);
	va_end(args);
}


void tinyfont_draw_char16x16(gfxScreen_t screen, int x, int y, unsigned int color, char c)
{
/*	  if(c == ' ') return;
	unsigned char *glyph = (unsigned char*)(msx_font + c * 8);
	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	int i, j;
	for (i = 0; i < 8; ++i, ++glyph) {
		for (j = 0; j < 8; ++j) {
			if ((*glyph & (128 >> j))) {
				TinyFont_Vertex *vertex = sceGuGetMemory(4 * sizeof(TinyFont_Vertex));
				register int i2 = i*2, j2 = j*2; 
				vertex[0] = (TinyFont_Vertex){x+j2,	  y+i2,	  0};
				vertex[1] = (TinyFont_Vertex){x+j2+1, y+i2,	  0};
				vertex[2] = (TinyFont_Vertex){x+j2,	  y+i2+1 ,0};
				vertex[3] = (TinyFont_Vertex){x+j2+1, y+i2+1, 0};
				sceGuDrawArray(GU_POINTS, GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertex);	
			}
		}
	}*/
}

void tinyfont_draw_string16x16(gfxScreen_t screen, int x, int y, unsigned int color, const char *string)
{
	/*
	if(string == NULL) return;
	int startx = x;
	const char *s = string;
	while(*s) {
		if(*s == '\n') {
			x = startx;
			y+=16;
		} else if(*s == '\t') {
			x+=16*4;
		} else {
			tinyfont_draw_char16x16(x, y, color, *s);
			x+=16;
		}
		++s;
	}*/
}

void tinyfont_draw_stringf16x16(gfxScreen_t screen, int x, int y, unsigned int color, const char *s, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, s);
	vsnprintf(buffer, 256, s, args);
	tinyfont_draw_string16x16(screen, x, y, color, buffer);
	va_end(args);
}
