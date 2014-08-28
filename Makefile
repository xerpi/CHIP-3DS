#use old CTRULIB
CTRULIB = /home/xerpi/Desktop/ctrulib-old/libctru

TARGET  = CHIP-3DS
OBJS    = crt0.o main.o chip-8.o utils.o PONG2.o

CC      = $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY = $(DEVKITARM)/bin/arm-none-eabi-objcopy
BIN2S   = $(DEVKITARM)/bin/bin2s
CFLAGS  = -Wall -std=c99 -march=armv6 -O3 -I"$(CTRULIB)/include"
LDFLAGS = -nostartfiles -nostdlib -T ccd00.ld -L"$(DEVKITARM)/arm-none-eabi/lib" -L"$(CTRULIB)/lib"
LIBS    = -lctru -lm

all: $(TARGET).3ds

CCI    := $(TARGET).3ds
RSF    := $(TARGET).rsf
ICON   := $(TARGET).icn
BANNER := $(TARGET).bnr
ROMFS  := $(TARGET).romfs

$(TARGET).3ds: $(TARGET).elf
	$(MAKEROM) -f cci -o $(CCI) -rsf $(RSF) -target d -exefslogo -elf $(TARGET).elf # -icon $(ICON) -banner $(BANNER)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@
%.s: %.ch8
	$(BIN2S) $< > $@

clean:
	@rm -rf $(OBJS) $(TARGET).3ds $(TARGET).elf $(TARGET).bin PONG2.s
