#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "readFile.h"
#include "createMap.h"
#include "plotSDL.h"

int main(int argc, char **argv)
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    init_map(map);
    range *bound = (range *)malloc(sizeof(range));

    // Load map from file
    char *filename = argv[1];
    int readresult = readMap(filename, map, bound);
    if (readresult != EXIT_NO_ERRORS)
    {
        return readresult;
    }
    path_t *path = (path_t*)malloc(sizeof(path_t));
    path->count = 0;
    path->pathCount = NULL;
    sdl(map,path);

    free_map(map);
    free(bound);
    return EXIT_NO_ERRORS;
}