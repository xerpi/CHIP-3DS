#ifndef CHIP3DS_H
#define CHIP3DS_H

#include <stdint.h>

struct chip3ds_context {
    struct {
        uint8_t  V[16];
        uint16_t I;
        uint16_t PC;
        uint8_t  SP;
        uint8_t  DT;
        uint8_t  ST;
    } regs;
    
    uint8_t RAM[0xFFF];
    uint16_t stack[16];
    
    uint16_t keyboard, old_keyboard;
    
    uint8_t *disp_mem;
    uint8_t disp_w;
    uint8_t disp_h;
    
    unsigned long long ticks;
};

void chip3ds_init(struct chip3ds_context *ctx, uint8_t display_w, uint8_t display_h);
void chip3ds_reset(struct chip3ds_context *ctx);
void chip3ds_fini(struct chip3ds_context *ctx);
void chip3ds_step(struct chip3ds_context *ctx);
void chip3ds_cls(struct chip3ds_context *ctx);
void chip3ds_key_press(struct chip3ds_context *ctx, uint8_t key);
void chip3ds_key_release(struct chip3ds_context *ctx, uint8_t key);
int  chip3ds_loadrom(struct chip3ds_context *ctx, char *path);
void chip3ds_core_dump(struct chip3ds_context *ctx);



#endif
