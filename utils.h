#ifndef UTILS_H
#define UTILS_H

#include <ctr/types.h>
#include <stddef.h>
#include "chip-8.h"

#ifdef __cplusplus
extern "C" {
#endif

int chip8_loadrom(struct chip8_context *ctx, char *path);
int chip8_loadrom_memory(struct chip8_context *ctx, void *addr, unsigned int size);
void print_bin(uint8_t n);


#define printf(...)
void *memcpy(void * destination, const void *source, size_t num);
void *memset(void * ptr, int value, size_t num);
size_t strlen(const char *str);
char *strcpy(char *destination, const char *source);
int ffs(int n);
int rand();


#ifdef __cplusplus
}
#endif

#endif
