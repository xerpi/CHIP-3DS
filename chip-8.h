#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct chip8_context {
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
};

void chip8_init(struct chip8_context *ctx, uint8_t display_w, uint8_t display_h);
void chip8_reset(struct chip8_context *ctx);
void chip8_fini(struct chip8_context *ctx);
void chip8_step(struct chip8_context *ctx);
void chip8_cls(struct chip8_context *ctx);
void chip8_key_press(struct chip8_context *ctx, uint8_t key);
void chip8_key_release(struct chip8_context *ctx, uint8_t key);
int  chip8_loadrom(struct chip8_context *ctx, char *path);
void chip8_core_dump(struct chip8_context *ctx);

#ifdef __cplusplus
}
#endif

#endif
