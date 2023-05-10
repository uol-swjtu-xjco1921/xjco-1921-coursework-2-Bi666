#include <stdio.h>
#include <stdlib.h>
#include "map.h"

node_t *get_node_by_id(map_t *map, int id);
void initsize(sizeMap_t *size);
void origin(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer);
void link(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer);
void plotpath(map_t *map, path_t *path, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer);
void axis(map_t *map, sizeMap_t *size, SDL_Window *window, SDL_Renderer *renderer, int option);
