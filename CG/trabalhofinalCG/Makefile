CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lGL -lGLU -lglut -lm
TARGET = simulador

# Ordem correta para resolver dependências
OBJS = utils.o types.o lighting.o terrain.o bolinhas.o camera.o obj_loader.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)
