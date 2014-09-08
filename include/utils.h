#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>
#include <stddef.h>
#include "chip-8.h"

#ifdef __cplusplus
extern "C" {
#endif

int chip8_loadrom(struct chip8_context *ctx, char *path);
int chip8_loadrom_memory(struct chip8_context *ctx, void *addr, unsigned int size);


#ifdef __cplusplus
}
#endif

#endif
