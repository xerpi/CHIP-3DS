TARGET = Chip-3DS
OBJS = main.o chip-3ds.o

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
