#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "readFile.h"
#include "plotSDL.h"

int main(int argc, char **argv)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    init_map(map);
    range_t *bound = (range_t *)malloc(sizeof(range_t));
    path_t *path = (path_t *)malloc(sizeof(path_t));

    if (argc == 1)
    {
        printf("Usage: %s input.map\n", argv[0]);
        return EXIT_NO_ERRORS;
    }
    // Nnumber of commandline does not match
    if (argc != 2)
    {
        printf("ERROR: Bad Argument Count\n");
        return EXIT_WRONG_ARG_COUNT;
    }

    // Load map from file
    char *filename = argv[1];
    int readresult = readMap(filename, map, bound);
    if (readresult != EXIT_NO_ERRORS)
    {
        freeMap(map, bound, path);
        return readresult;
    }
    path->count = 0;
    path->pathCount = NULL;
    int sdlresult = sdl(map, path, bound);
    if (sdlresult != EXIT_NO_ERRORS)
    {
        freeMap(map, bound, path);
        return sdlresult;
    }

    freeMap(map, bound, path);
    return EXIT_NO_ERRORS;
}