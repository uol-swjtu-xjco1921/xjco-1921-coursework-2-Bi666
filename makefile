CC = gcc
CFLAGS = -lm `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf

SRC = createMap.c readFile.c routePlan.c plotSDL.c dijkstra.c optionSDL.c shortSDL.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = map1

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXECUTABLE)