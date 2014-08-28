#use old CTRULIB
CTRULIB = /home/xerpi/Desktop/ctrulib-old/libctru

TARGET  = CHIP-3DS
OBJS    = crt0.o main.o chip-8.o utils.o PONG2.o

CC      = $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY = $(DEVKITARM)/bin/arm-none-eabi-objcopy
BIN2S   = $(DEVKITARM)/bin/bin2s
CFLAGS  = -Wall -std=c99 -march=armv6 -O3 -I"$(CTRULIB)/include"
LDFLAGS = -nostartfiles -nostdlib -T ccd00.ld -L"$(DEVKITARM)/arm-none-eabi/lib" -L"$(CTRULIB)/lib"
LIBS    = -lctru -lc -lm

ELF    := $(TARGET).elf
CCI    := $(TARGET).3ds
CXI    := $(TARGET).cxi
CIA    := $(TARGET).cia
RSF    := $(TARGET).rsf
ICON   := $(TARGET).icn
BANNER := $(TARGET).bnr
ROMFS  := $(TARGET).romfs

all: $(CCI) $(CIA)

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) $(filter-out crt0.o, $^) -o $@ $(LIBS)
$(CCI): $(ELF)
	$(MAKEROM) -f cci -o $(CCI) -rsf $(RSF) -target d -exefslogo -elf $(TARGET).elf # -icon $(ICON) -banner $(BANNER)
$(CXI): $(ELF)
	$(MAKEROM) -elf $(TARGET).elf -rsf $(RSF)
$(CIA): $(CXI)
	$(MAKEROM) -f cia -content $(CXI):0:0 -o $(CIA)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@
%.s: %.ch8
	$(BIN2S) $< > $@

clean:
	@rm -rf $(OBJS) $(ELF) $(CCI) $(CXI) $(CIA)
