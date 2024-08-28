CC = gcc
VCPKG_ROOT = D:/Users/moham/OneDrive/Documents/GitHub/vcpkg
CFLAGS = -Iinclude -Iinclude1 -I${VCPKG_ROOT}/installed/x64-windows/include
LDFLAGS = -Llib -L${VCPKG_ROOT}/installed/x64-windows/lib 
LIBS = -lSDL2 -lm -lopengl32

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

all: main

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) main
