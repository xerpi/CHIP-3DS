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

int chip8_loadrom_memory(struct chip8_context *ctx, const void *addr, unsigned int size)
{
	memcpy(&(ctx->RAM[0x200]), addr, size);
	return size;
}

