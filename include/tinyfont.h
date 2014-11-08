/*
* Copyright (C) 2014, xerpi
*/


#ifndef _TINYFONT_H_
#define _TINYFONT_H_

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI
	#define PI (3.14159265f)
#endif
#ifndef _2PI
	#define _2PI (6.28318531f)
#endif


void tinyfont_draw_char(gfxScreen_t screen, int x, int y, unsigned int color, char c);
void tinyfont_draw_string(gfxScreen_t screen, int x, int y, unsigned int color, const char *string);
void tinyfont_draw_stringf(gfxScreen_t screen, int x, int y, unsigned int color, const char *s, ...);

void tinyfont_draw_char16x16(gfxScreen_t screen, int x, int y, unsigned int color, char c);
void tinyfont_draw_string16x16(gfxScreen_t screen, int x, int y, unsigned int color, const char *string);
void tinyfont_draw_stringf16x16(gfxScreen_t screen, int x, int y, unsigned int color, const char *s, ...);


#ifdef __cplusplus
}
#endif

#endif
