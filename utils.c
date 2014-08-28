#include "utils.h"

int chip8_loadrom(struct chip8_context *ctx, char *path)
{
    /*FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    long read = fread(&(ctx->RAM[0x200]), 1, size, fp);
    fclose(fp);
    return (size == read);*/
    return 0;
}

int chip8_loadrom_memory(struct chip8_context *ctx, void *addr, unsigned int size)
{
    memcpy(&(ctx->RAM[0x200]), addr, size);
    return size;
}

void print_bin(uint8_t n)
{
    int i;
    for (i = 0; i < 8; i++) {
        printf("%c", (n>>(7-i))&0b1 ? 'x' : 'o');
    }
}

void *memcpy(void *destination, const void *source, size_t num)
{
    const uint8_t *src = source;
    uint8_t *dst = destination;
    while (num--) {
        *dst++ = *src++;
    }
    return destination;
}

void *memset(void *ptr, int value, size_t num)
{
    uint8_t *p = ptr;
    while (num--) {
        *p++ = value;
    }
    return ptr;
}

int ffs(int n)
{
    int cnt = 0;
    while (!(n & 0b1)) {
        cnt++;
        n>>=1;
    }
    return cnt;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

char *strcpy(char *destination, const char *source)
{
    char *dst = destination;
    while (*source) {
        *dst++ = *source++;
    }
    *dst = '\0';
    return destination;
}

int rand()
{
    return 0;
}
