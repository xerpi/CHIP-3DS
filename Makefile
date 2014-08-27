TARGET = CHIP-3DS
OBJS = main.o utils.o chip-8.o

CC = gcc
CFLAGS = -O2 -Wall
LIBS = -lSDL2

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(TARGET) $(OBJS)
